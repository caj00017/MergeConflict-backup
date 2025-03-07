#ifndef CONTEXT_H
#define CONTEXT_H

#include <mpx/pcb.h>

/**
 * @brief File for context structure.
 * @authors Chris Jones,
 */

typedef struct context{
    // segment registers
    int CS, DS, ES, FS, GS, SS;

    // status control registers
    int EIP, EFLAGS;

    // general purpose registers
    int EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP;

    // pointer to the pcb this belongs to
    struct pcb* pcb;

} context;

#endif
