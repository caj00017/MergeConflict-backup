#include <mpx/sys_call.h>

// global PCB pointer
pcb* GLOBAL_PCB = (void*)0;

context* sys_call(context* ctx) {
    pcb* thisPCB = ctx->pcb;
    pcb* nextPCB = thisPCB->next_node;
    GLOBAL_PCB = nextPCB;
    unsigned char* return_ctx = nextPCB->ctx_ptr;
    return (context*)return_ctx;
}
