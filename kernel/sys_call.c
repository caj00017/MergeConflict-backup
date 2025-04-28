#include <mpx/sys_call.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <mpx/pcb.h>

// global PCB pointer
pcb* CURRENT_PCB = NULL;

// global context pointer
context* GLOBAL_CTX = NULL;

// next process tracker
pcb* nextPCB = NULL;

dcb* current_dcb = NULL;


context* sys_call(context* ctx) {

    // get the operation code from the context
    int op = ctx->EAX;
    int dev = ctx->EBX;
    current_dcb = get_dcb(dev);

    iocb* cur_iocb = current_dcb->queue_head;

    while(cur_iocb != NULL){
        if(cur_iocb->event_flag == 1){
            IO_Completion(ctx, cur_iocb);
        }
        cur_iocb = cur_iocb->next;
    }

    // if there is a ready process in the queue, set it to the next process
    if(return_queue(0) != NULL) {
        nextPCB = find_first_ready();
    }
    
    if(op == 0) /* EXIT */{
        //Delete Currently Running PCB
        pcb_free(CURRENT_PCB);
        CURRENT_PCB = NULL;

        //check if there is a ready process in the queue
        if (nextPCB != NULL) {
            //remove the process from the queue to be exited
            pcb_remove(nextPCB);
            CURRENT_PCB = nextPCB;
            //set its state to runnning
            CURRENT_PCB->state = RUNNING;
            //save the context of the process while it is being run
            context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
            //make sure the return value seen by sys_req is zero
            next_ctx->EAX = 0;
            //return the running process' context
            return next_ctx;
        }
        else {
            //if there is no ready process in the queue, return the global context
            context* next_ctx = GLOBAL_CTX;
            //make sure the value seen by sys_req is zero
            next_ctx->EAX = 0;
            return GLOBAL_CTX;
        }
    }

    else if (op == 1) /* IDLE */ {

        //Save Global Context
        if(GLOBAL_CTX == NULL) {
            GLOBAL_CTX = ctx;
        }
        //as long as the current pcb is not null, store the context of the current pcb and insert it back into the queue before the next
        //process is run (context switch)
        if(CURRENT_PCB != NULL){
            CURRENT_PCB->stack_ptr = (unsigned char*)ctx;
            CURRENT_PCB->state = READY_NOT_SUS;
            pcb_insert(CURRENT_PCB);
            CURRENT_PCB = NULL;
        }

         //if any nonsuspended PCBs in queue, remove first from queue, store in temp variable as next process
        if(nextPCB != NULL){
            //remove the next process from the queue
            pcb_remove(nextPCB);
            
            //set the current pcb to the next process if there is no process running
            CURRENT_PCB = nextPCB;
            //set its state to running in the system
            CURRENT_PCB->state = RUNNING;
            //return the context of the next process
            context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
            next_ctx->EAX = 0;
            return next_ctx;
        }
        else {
            //if there are no ready processes in the queue, return the global context of the first process run in the system
            context* next_ctx = GLOBAL_CTX;
            next_ctx->EAX = 0;
            return next_ctx;
        }

    }
    else if (op == 2) /* READ */{
        if(current_dcb->status == 0){
            serial_read(current_dcb->dev, current_dcb->queue_head->buffer, current_dcb->queue_head->length);
        }
        else{
            IO_Scheduler(ctx, 2);
        }
        context *return_ctx = ctx;
        return return_ctx;
    }
    
    else if (op == 3) /* WRITE */{
        if(current_dcb->status == 0){
            serial_write(current_dcb->dev, current_dcb->queue_head->buffer, current_dcb->queue_head->length);
            
        }
        else{
            IO_Scheduler(ctx, 3);
        }

        context *return_ctx = ctx;
        return return_ctx;
    }
    //shouldn't reach here
    else {
        //in that case we just need to make sure that we return the current context
        //and also make sure that the return value seen by sys_req is -1
        ctx->EAX = -1;
        context *return_ctx = ctx;
        return return_ctx;
    }
}

pcb* find_first_ready(void) {
    //find the head of the non-suspended ready queue
    queue* ready = return_queue(0);
    //get the first pcb in the queue
    pcb* current_pcb = ready->head;
    //iterate through the queue until a ready pcb is found
    while (current_pcb != NULL) {
        //return the first ready pcb that is not suspended
        if (current_pcb->state == READY_NOT_SUS) {
            return current_pcb;
        }
        current_pcb = current_pcb->next_node;
    }
    return NULL;
}

int IO_Scheduler(context* ctx, int op_code) {

    //checking to make sure op is READ or WRITE
    if(op_code == 0 || op_code ==  1){
        return -1;
    }

    //checking to make sure the dev is a recognized one
    if(ctx->EBX != COM1 && ctx->EBX != COM2 && ctx->EBX != COM3 && ctx->EBX != COM4){
        return -1;
    }

    //checking to make usre the dcb isn't NULL
    if(current_dcb == NULL){
        return -1;
    }

    // //checking to make sure the buffer isn't NULL
    // if(ctx->ECX == NULL){
    //     return -1;
    // }

    //Checking to make sure the size is greater than 0
    if(ctx->EDX <= 0){
        return -1;
    }

    if(current_dcb->queue_head->process == NULL){
        
        current_dcb->queue_head->process = CURRENT_PCB;
        current_dcb->queue_head->buffer = (void*)ctx->ECX;
        current_dcb->queue_head->length = ctx->EBX;

        if(op_code == 2){
            serial_input_interrupt(current_dcb->dev);
        }
        else{
            serial_output_interrupt(current_dcb->dev);
        }
    }
    else if(current_dcb->queue_head != NULL){
        iocb* current_iocb = current_dcb->queue_head;
        while(current_iocb->next != NULL){
            current_iocb = current_iocb->next;
        }
        iocb* new_iocb = iocb_setup(CURRENT_PCB, (void*)ctx->ECX, ctx->EDX, op_code);
        current_iocb->next = new_iocb;
    }

    return 0;
}

void IO_Completion(context* ctx, iocb* current_iocb){
   
   pcb_remove(current_iocb->process);
   current_iocb->process->state = 1;
   pcb_insert(current_iocb->process);

   ctx->EAX = current_iocb->transferred;

   iocb_clear(current_iocb);

   if(current_dcb->queue_head != NULL){
        IO_Scheduler(ctx, current_dcb->queue_head->next->operation);
   }

}
