#include <mpx/sys_call.h>

// global PCB pointer
pcb* GLOBAL_PCB = (void*)0;

struct context* sys_call(context* ctx) {
    pcb* thisPCB = ctx->pcb;
    pcb* nextPCB = thisPCB->next_node;
    GLOBAL_PCB = nextPCB;
    return nextPCB->ctx_ptr;
}
