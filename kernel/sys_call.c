#include <mpx/sys_call.h>

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
        CURRENT_PCB = NULL;

        if (nextPCB != NULL) {
            pcb_remove(nextPCB);
            CURRENT_PCB = nextPCB;
            CURRENT_PCB->state = RUNNING;
            context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
            next_ctx->EAX = 0;
            return next_ctx;
        }
        else {
            context* next_ctx = GLOBAL_CTX;
            next_ctx->EAX = 0;
            return GLOBAL_CTX;
        }
    }

    else if (op == 1) /* IDLE */ {

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
            if (CURRENT_PCB == NULL) {
                CURRENT_PCB = nextPCB;
                CURRENT_PCB->state = RUNNING;
                context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
                next_ctx->EAX = 0;
                return next_ctx;
            }
            else {
                CURRENT_PCB = nextPCB;
                CURRENT_PCB->state = RUNNING;
                context* next_ctx = (context*) CURRENT_PCB->stack_ptr;
                next_ctx->EAX = 0;
                return next_ctx;
            }
        }
        else {
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
