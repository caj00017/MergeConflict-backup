#include <context.h>
#include <mpx/pcb.h>
#include <stddef.h>
#include <sys_req.h>

// global PCB pointer
extern pcb* GLOBAL_PCB;

//global context pointer
extern context* GLOBAL_CTX;

/**
 * 
 * @authors Chris Jones, Evan Humphrey
 */
context* sys_call(context* ctx);
