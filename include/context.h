#ifndef CONTEXT_H
#define CONTEXT_H

/**
 * @brief File for context structure.
 * @authors Chris Jones,
 */

 typedef struct {
    
    //general purpose registers
    int EAX, EBX, ECX, EDX, ESI, EDI, EBP;

    //segment registers
    int DS, ES, FS, GS, SS;

    //status control registers
    int EIP, CS, EFLAGS;

    // name of the PCB this context belongs to
    char* pcb_name;

} context;

#endif
