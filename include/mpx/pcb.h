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
    unsigned char* stack;
    unsigned char* stack_location; // points to the final byte in the stack
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
 * @brief Set the name of the pcb
 * @param pcb* pcb The pcb to set the name of
 * @param char* name The name to set
 * @returns Status code
 * @author Chris Jones
 */
int set_name(pcb* pcb, char* name);

/**
 * @brief Get the class of the pcb
 * @param pcb* pcb The pcb to get the class of
 * @returns int The class of the pcb (0 - user process, 1 - kernel process)
 * @author Chris Jones
 */
int get_class(pcb* pcb);

/**
 * @brief Set the class of the pcb
 * @param pcb* pcb The pcb to set the class of
 * @param int class The class to set
 * @returns Status code
 * @author Chris Jones
 */
int set_class(pcb* pcb, int class);

/**
 * @brief Get the priority of the pcb
 * @param pcb* pcb The pcb to get the priority of
 * @returns int The priority level of the pcb (0-9)
 * @author Chris Jones
 */
int get_priority(pcb* pcb);

/**
 * @brief Set the priority of the pcb
 * @param pcb* pcb The pcb to set the priority of
 * @param int priority The priority to set
 * @returns Status code
 * @author Chris Jones
 */
int set_priority(pcb* pcb, int priority);

/**
 * @brief Get the state of the pcb
 * @param pcb* pcb The pcb to get the state of
 * @returns int The state of the pcb (0-7)
 * @author Chris Jones
 */
int get_state(pcb* pcb);

/**
 * @brief Set the state of the pcb
 * @param pcb* pcb The pcb to set the state of
 * @param int state The state to set
 * @returns Status code
 * @author Chris Jones
 */
int set_state(pcb* pcb, int state);

/**
 * @brief Get the execution mode of the pcb
 * @param pcb* pcb The pcb to get the execution mode of
 * @returns int The execution mode of the pcb
 * @author Chris Jones
 */
int get_execution_mode(pcb* pcb);

/**
 * @brief Set the execution mode of the pcb
 * @param pcb* pcb The pcb to set the execution mode of
 * @param int execution_mode The execution mode to set
 * @returns Status code
 * @author Chris Jones
 */
int set_execution_mode(pcb* pcb, int execution_mode);

/**
 * @brief Get the name of the pcb
 * @param pcb* pcb The pcb to get the name of
 * @returns int The dispatch mode of the pcb
 * @author Chris Jones
 */
int get_dispatch_mode(pcb* pcb);

/**
 * @brief Set the dispatch mode of the pcb
 * @param pcb* pcb The pcb to set the dispatch mode of
 * @param int dispatch_mode The dispatch mode to set
 * @returns Status code
 * @author Chris Jones
 */
int set_dispatch_mode(pcb* pcb, int dispatch_mode);

/**
 * @brief Get the stack location of the pcb
 * @param pcb* pcb The pcb to get the stack location of
 * @returns unsigned char* The stack location of the pcb
 * @author Chris Jones
 */
unsigned char* get_stack_location(pcb* pcb);

/**
 * @brief Set the stack location of the pcb
 * @param pcb* pcb The pcb to set the stack location of
 * @param unsigned char* stack_location The stack location to set
 * @returns Status code
 */
int set_stack_location(pcb* pcb, unsigned char* stack_location);

/**
 * @brief Get the next node in the pcb queue
 * @param pcb* pcb The current pcb
 * @returns pcb* The next node in the pcb queue
 * @author Chris Jones
 */
pcb* get_next_node(pcb* pcb);

/**
 * @brief Set the next node in the pcb queue
 * @param pcb* pcb The current pcb
 * @param pcb* next_node The next node in the pcb queue
 * @returns int Status code
 * @author Chris Jones
 */
int set_next_node(pcb* pcb, pcb* next_node);

/**
 * @brief Get the previous node in the pcb queue
 * @param pcb* pcb The current pcb
 * @returns pcb* The previous node in the pcb queue
 * @author Chris Jones
 */
pcb* get_prev_node(pcb* pcb);

/**
 * @brief Set the previous node in the pcb queue
 * @param pcb* pcb The current pcb
 * @param pcb* prev_node The previous node in the pcb queue
 * @returns int Status code
 * @author Chris Jones
 */
int set_prev_node(pcb* pcb, pcb* prev_node);

#endif