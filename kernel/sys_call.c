#include <mpx/sys_call.h>
#include <mpx/serial.h>

// global PCB pointer
pcb* CURRENT_PCB = NULL;

// global context pointer
context* GLOBAL_CTX = NULL;

// next process tracker
pcb* nextPCB = NULL;

context* sys_call(context* ctx) {

    // get the operation code from the context
    int op = ctx->EAX;

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

    // }
    // else if (op == 2) /* READ */{
    //     if(ctx->EBX->status == 0){
    //         serial_read();
    //     }
    // }
    
    // else if (op == 3) /* WRITE */{
    //     if(ctx->EBX->status == 0){
    //         serial_write();
    //     }
    // }
    //if the op code is 2 or 3, then the call is to READ or WRITE
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

// void IO_Scheduler(context* ctx) {
//     if(op_code == 0 || op_code ==  1){
//         return;
//     }
// }