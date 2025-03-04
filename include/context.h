#ifndef CONTEXT_H
#define CONTEXT_H

/**
 * @brief File for context structure.
 * @authors Chris Jones,
 */

typedef struct {
    // segment registers
    uint16_t CS, DS, ES, FS, GS, SS;

    // status control registers
    uint32_t EIP, EFLAGS;

    // general purpose registers
    uint32_t EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP;

} context;

#endif