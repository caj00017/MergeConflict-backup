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
 * @authors Evan Humphrey, Izaak Whetsell, Chris Jones, Tanner Forbes
 */
context* sys_call(context* ctx);

/**
 * @brief finds the first process in the ready queue
 * @returns the first process in the ready queue
 * @authors Chris Jones, Evan Humphrey
 */
pcb* find_first_ready(void);

/**
 * @brief Processes Input and Output Requests
 * @param ctx the context of the current process
 * @param op_code the op_code of the current request
 * @return an code of whether it scheduled successfully or not
 * @author Tanner Forbes
 */
int IO_Scheduler(context* ctx, int op_code);

/**
 * @brief Completes the IO request when flag is signaled
 * @param ctx the context of the current process
 * @param current_iocb the current iocb whose IO request has been completed
 * @author Tanner Forbes
 */
void IO_Completion(context* ctx, iocb* current_iocb);
