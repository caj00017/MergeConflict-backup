#include <context.h>
#include <mpx/pcb.h>
#include <stddef.h>
#include <sys_req.h>
#include <mpx/serial.h>

// global PCB pointer
extern pcb* CURRENT_PCB;

// global context pointer
extern context* GLOBAL_CTX;

// next process tracker
extern pcb* nextPCB;

/**
 * @brief system call function that handles all system calls from the user
 * @param ctx the context of the current process
 * @return the context of the next process
 * @authors Evan Humphrey, Izaak Whetsell, Chris Jones
 */
context* sys_call(context* ctx);

/**
 * @brief finds the first process in the ready queue
 * @returns the first process in the ready queue
 * @authors Chris Jones, Evan Humphrey
 */
pcb* find_first_ready(void);

int IO_Scheduler(context* ctx, int op_code);

void IO_Completion(context* ctx, iocb* current_iocb);
