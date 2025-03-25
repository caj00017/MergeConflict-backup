#include <mpx/sys_call.h>
#include <stddef.h>
#include <mpx/pcb.h>

//Function protos
pcb* find_first_ready(void);

//global PCB pointer
pcb* GLOBAL_PCB = NULL;
pcb* CURRENT_PCB = NULL;

//global context pointer
context* GLOBAL_CTX = NULL;

// current process tracker
// pcb* curPCB = NULL;
pcb* nextPCB = NULL;

context* sys_call(context* ctx) {
    int op = ctx->EAX;

    if(return_queue(0) != NULL) {
        nextPCB = find_first_ready();
    }


    if(op == 0) /* EXIT */{
        //Delete Currently Running PCB

        //See if it exists in queue
        // if(pcb_find(CURRENT_PCB->name) != NULL){ 
        //     //Remove from queue
        //     pcb_remove(CURRENT_PCB);
        // }

        CURRENT_PCB = NULL;
        //Do we need to free the memory aswell??

        if(nextPCB != NULL){
            pcb_remove(nextPCB);
            
            CURRENT_PCB = nextPCB;
            CURRENT_PCB->state = RUNNING;
            context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
            next_ctx->EAX = 0;
            return next_ctx;
        
        }
        else{
            context* next_ctx = GLOBAL_CTX;
            next_ctx->EAX = 0;
            return GLOBAL_CTX;
        }

       
    }

    else if(op == 1) /* IDLE */ {
        //Save Global Context
        if(GLOBAL_CTX == NULL) {
            GLOBAL_CTX = ctx;
        }

        if(CURRENT_PCB != NULL){
            CURRENT_PCB->stack_ptr = (unsigned char*)ctx;

            CURRENT_PCB->state = READY_NOT_SUS;

            pcb_insert(CURRENT_PCB);

            CURRENT_PCB = NULL;
        }

        if(return_queue(0) != NULL) {
            nextPCB = find_first_ready();
        }

         //if any nonsuspended PCBs in queue, remove first from queue, store in temp variable as next process
        if(nextPCB != NULL){
            pcb_remove(nextPCB);
           
            if(CURRENT_PCB == NULL){
                CURRENT_PCB = nextPCB;
                CURRENT_PCB->state = RUNNING;
                context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
                next_ctx->EAX = 0;
                return next_ctx;
            }
            else{
                // CURRENT_PCB->stack_ptr = (unsigned char*)ctx;

                // CURRENT_PCB->state = READY_NOT_SUS;

                // pcb_insert(CURRENT_PCB);

                CURRENT_PCB = nextPCB;

                CURRENT_PCB->state = RUNNING;
                context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
                next_ctx->EAX = 0;
                return next_ctx;
            }

        }
        else{

            context* next_ctx = GLOBAL_CTX;
            next_ctx->EAX = 0;
            return next_ctx;
        }

    }

    else {
        ctx->EAX = -1;
        context *return_ctx = ctx;
        return return_ctx;
    }




    /*OLD CODE*/
    // int op = ctx->EAX;
    // if(return_queue(0) != NULL) {
    //     GLOBAL_PCB = find_first_ready();
    // }

    // if(op == 0) /* EXIT */{
    //     //delete the currently running pcb
    //     pcb* curPCB = GLOBAL_PCB;
    //     pcb_remove(curPCB);

    //     //If there are any ready, non-suspended PCBs in the queue, load the first as in IDLE
    //     if(find_first_ready()) {
    //         //Load the first ready, non-suspended PCB
    //         GLOBAL_PCB = find_first_ready();
    //         ctx->EAX = 0;
    //         return (context*)GLOBAL_PCB->stack_ptr;
    //     }

    //     //If the PCB queue is empty, or only consists of blocked or suspended PCBs, load the original context
    //     else {
    //         GLOBAL_PCB = curPCB;
    //         ctx-> EAX = 0;
    //         return GLOBAL_CTX;
    //     }

    //     // In all cases, ensure that the return value seen by sys req() is 0
    // }
    // else if(op == 1) /* IDLE */ {
    //     if(GLOBAL_CTX == NULL) {
    //         GLOBAL_CTX = ctx;
    //     }
        
    //     // if there is a runnning process
    //     if(GLOBAL_PCB != NULL) {

    //         pcb_remove(GLOBAL_PCB);

    //         if(curPCB == NULL) {
    //             curPCB = GLOBAL_PCB;
    //             return (context*)curPCB->stack_ptr;
    //         }
    //         else {
    //             curPCB->stack_ptr = (unsigned char*)ctx;
    //             //set state to ready
    //             pcb_insert(curPCB);
    //             curPCB = GLOBAL_PCB;
    //             //set state to running
    //             return (context*)curPCB->stack_ptr;
    //         }
    //     }
    //     else {
    //         ctx->EAX = 0;
    //         return GLOBAL_CTX;
    //     }
    // }
    // else {
    //     ctx->EAX = -1;
    //     context *return_ctx = ctx;
    //     return return_ctx;
    // }




}

pcb* find_first_ready(void) {
    queue* ready = return_queue(0);
    pcb* current_pcb = ready->head;
    while (current_pcb != NULL) {
        if (current_pcb->state == READY_NOT_SUS) {
            return current_pcb;
        }
        current_pcb = current_pcb->next_node;
    }
    return NULL;
}
