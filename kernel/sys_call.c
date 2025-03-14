#include <mpx/sys_call.h>
#include <stddef.h>
#include <mpx/pcb.h>

//Function protos
pcb* find_first_ready(void);

// global PCB pointer
pcb* GLOBAL_PCB = NULL;

context* sys_call(context* ctx) {
    int op = ctx->EAX;
    if(op == 0) /* EXIT */{
        //delete the currently running pcb
        pcb* curPCB = GLOBAL_PCB;
        pcb_remove(curPCB);

        //If there are any ready, non-suspended PCBs in the queue, load the first as in IDLE
        if(find_first_ready()) {
            //Load the first ready, non-suspended PCB
            GLOBAL_PCB = find_first_ready();
            ctx->EAX = 0;
            return (context*)GLOBAL_PCB->stack_ptr;
        }

        //If the PCB queue is empty, or only consists of blocked or suspended PCBs, load the original context
        else {
            GLOBAL_PCB = curPCB;
            ctx-> EAX = 0;
            return (context*)curPCB->stack_ptr;
        }

        // In all cases, ensure that the return value seen by sys req() is 0
    }
    else if(op == 1) /* IDLE */ {
        // If there are any ready, non-suspended PCBs in the queue, 
        // remove the first from the queue and store it in a temporary variable as the next process
        pcb* curPCB = GLOBAL_PCB;
        if(find_first_ready()) {
            pcb* nextPCB = find_first_ready();
            pcb_remove(nextPCB);

            // Save the context of the current PCB by updating its stack pointer
            ctx->EBP = (int)curPCB->stack_ptr; //MAYBE?
            curPCB->stack_ptr = curPCB->stack + 1024 - sizeof(ctx) - 2;

            // Add the current PCB back to the queue
            pcb_insert(curPCB);

            // Return the context of the next process
            ctx->EAX = 0;
            return (context*)nextPCB->stack_ptr;

        }
        else {
            // If the PCB queue is empty, or only consists of blocked or suspended PCBs, continue with the current process
            // In all cases, ensure that the return value seen by sys req() is 0
            ctx->EAX = 0;
            return ctx;

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
