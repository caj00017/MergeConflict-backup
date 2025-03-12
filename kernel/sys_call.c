#include <mpx/sys_call.h>

// global PCB pointer
pcb* GLOBAL_PCB = NULL;

context* sys_call(context* ctx) {

    // handle sys_req(IDLE) and (EXIT)
    int op = ctx->EAX;
    if (op == 0) { // EXIT
        // delete the currently running PCB
        pcb* thisPCB = pcb_find(ctx->pcb_name);
        pcb_remove(thisPCB);

        // if there are any ready, non-suspended PCBs in the queue,
        // remove the first from the queue and store it in a temporary
        // variable as the next process


        // if the PCB queue is empty, or only consists of blocked or
        // suspended PCBs, load the original context

        // ensure that the return value seen by sys_req is 0.
    }
    else if (op == 1) { // IDLE
        // if there are any ready, non-suspended PCBs in the queue,
        // remove the first from the queue and store it in a temporary
        // variable as the next process

        // save the context of the current PCB by updatings its 
        // stack pointer

        // add the current PCB back to the queue

        // return the context of the next process

        // if the PCB queue is empty, or only consists of blocked or
        // suspended PCBs, continue with the current process

        // in all cases, ensure that the return value seen by sys_req is 0.
    }


    pcb* thisPCB = pcb_find(ctx->pcb_name);
    pcb* nextPCB = thisPCB->next_node;
    GLOBAL_PCB = nextPCB;
    context* return_ctx = nextPCB->ctx_ptr;
    return return_ctx;
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
