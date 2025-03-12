#ifndef PCB_H
#define PCB_H

#include <context.h>

/**
 * @file pcb.h 
 * @brief Header file for pcb struct, getters/setters for fields
 * @authors Chris Jones, Evan Humphrey, Tanner Forbes
 */

/**
 * @brief PCB states
 */
enum pcb_state {
    READY_SUS, // =0
    READY_NOT_SUS, // =1
    RUNNING, // =2
    BLOCKED_SUS, // =3
    BLOCKED_NOT_SUS, // =4
};

typedef struct pcb {
    char* name;
    int class; /* 0 - user process, 1 - kernel process */
    int priority; /* 0-9 */
    int state; /* 0-4 */  //possible change to enum.
    unsigned char* stack; // points to the first byte in the stack
    unsigned char* stack_ptr; // points to the final byte in the stack
    context* ctx_ptr; // points to the location of this process's context
    struct pcb* next_node;
    struct pcb* prev_node;
} pcb;

typedef struct queue {
    int queue_type; // 0 - ready, 1 - blocked
    pcb* head;
    pcb* tail;
} queue;

/**
 * @brief Returns the queue based on the class
 * @param class Class of the queue to return (0 - ready, 1 - blocked)
 * @returns queue* pointer to the queue of the given class, NULL if invalid class is given
 * @author Chris Jones
 */
queue* return_queue(int class);

/**
 * @brief Sets up a pcb with the given name, class, and priority
 * @param name Name of the pcb
 * @param class Class of the pcb
 * @param priority Priority of the pcb
 * @returns pcb* pointer to the new pcb
 * @author Chris Jones
 */
pcb* pcb_setup(char* name, int class, int priority, void (*function_ptr)(void));

/**
 * @brief Allocate memory for a pcb
 * @returns pcb pointer to new allocated memory
 */
pcb* pcb_allocate(void);

/**
 * @brief Frees memory used by a given pcb
 * @param free_pcb* pcb to free memory of
 * @returns int which indicates 0 on sucesss and 1 on error
 */
int pcb_free(struct pcb* free_pcb);


/**
 * @brief Finds the PCB with the passed name in the queues
 * @param name Name of the PCB to be found
 * @returns pcb* is a pointer to the PCB's location
 */
pcb* pcb_find(const char * name);

/**
 * @brief Inserts PCB to the appropriate queue based on state and priority
 * @param pcbPtr Pointer to the PCB to be inserted
 */
void pcb_insert(pcb* pcbPtr);

/**
 * @brief Removes PCB from queue but doesn't delete it
 * @param pcbPtr Pointer to the PCB to be removed
 * @returns int which indicates 0 on sucesss and 1 on error
 */
int pcb_remove(pcb* pcbPtr);

#endif
