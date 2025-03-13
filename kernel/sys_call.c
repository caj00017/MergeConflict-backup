#include <mpx/sys_call.h>

// global PCB pointer
pcb* GLOBAL_PCB = (void*)0;

context* sys_call(context* ctx) {
  //  pcb* thisPCB = pcb_find(ctx->pcb_name);
  //  pcb* nextPCB = thisPCB->next_node;
   // GLOBAL_PCB = nextPCB;
    ctx->EAX = -1;
    context *return_ctx = ctx;
    return return_ctx;
}
