#ifndef PCB_H
#define PCB_H

/**
 * @file pcb.h 
 * @brief Header file for pcb struct, getters/setters for fields
 * @authors Chris Jones
 */

typedef struct {
    char* name;
    int class; /* 0 - user process, 1 - kernel process */
    int priority; /* 0-9 */
    int state; /* 0-7 */
    int execution_mode; /*  */
    int dispatch_mode; /*  */
    char* stack;
    void* stack_location; // points to the final byte in the stack
    pcb* next_node;
    pcb* prev_node;
} pcb;

/**
 * @brief Get the name of the pcb
 * @param pcb* pcb The pcb to get the name of
 * @returns char* The name of the pcb
 * @author Chris Jones
 */
char* get_name(pcb* pcb);

/**
 * @brief Get the class of the pcb
 * @param pcb* pcb The pcb to get the class of
 * @returns int The class of the pcb (0 - user process, 1 - kernel process)
 * @author Chris Jones
 */
int get_class(pcb* pcb);

/**
 * @brief Get the priority of the pcb
 * @param pcb* pcb The pcb to get the priority of
 * @returns int The priority level of the pcb (0-9)
 * @author Chris Jones
 */
int get_priority(pcb* pcb);

/**
 * @brief Get the state of the pcb
 * @param pcb* pcb The pcb to get the state of
 * @returns int The state of the pcb (0-7)
 * @author Chris Jones
 */
int get_state(pcb* pcb);

/**
 * @brief Get the execution mode of the pcb
 * @param pcb* pcb The pcb to get the execution mode of
 * @returns int The execution mode of the pcb
 * @author Chris Jones
 */
int get_execution_mode(pcb* pcb);

/**
 * @brief Get the name of the pcb
 * @param pcb* pcb The pcb to get the name of
 * @returns int The dispatch mode of the pcb
 * @author Chris Jones
 */
int get_dispatch_mode(pcb* pcb);

/**
 * @brief Get the stack location of the pcb
 * @param pcb* pcb The pcb to get the stack location of
 * @returns void* The stack location of the pcb
 * @author Chris Jones
 */
void* get_stack_location(pcb* pcb);

/**
 * @brief Get the next node in the pcb queue
 * @param pcb* pcb The current pcb
 * @returns pcb* The next node in the pcb queue
 * @author Chris Jones
 */
pcb* get_next_node(pcb* pcb);

/**
 * @brief Get the previous node in the pcb queue
 * @param pcb* pcb The current pcb
 * @returns pcb* The previous node in the pcb queue
 * @author Chris Jones
 */
pcb* get_prev_node(pcb* pcb);

#endif