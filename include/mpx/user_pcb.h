#ifndef U_PCB
#define U_PCB

#include <mpx/pcb.h>

/**
 @file mpx/user_pcb.h
 @brief User macros to create, alter, sort, or display PCB Information
*/

/**
 * @brief Creates PCB
 * @param name Name of the process to create
 * @param class Class of the process to create
 * @param priority Priority of the process to create
 * @returns Status code (0 success, -1 invalid state)
 */
int create_PCB(char* name, int class, int priority);

/**
 * @brief Deletes PCB from memory
 * @param name Name of the process to delete
 * @returns Status code (0 success, -1 invalid state)
 */
int delete_PCB(char* name);


 /**
 * @brief Move PCB to blocked state
 * @param name Name of the process to block
 * @returns Status code (0 success, -1 invalid state)
 */
int block_PCB(char* name);

 /**
 * @brief Moves PCB to the ready state
 * @param name Name of the process to unblock
 * @returns Status code (0 success, -1 invalid state)
 */
int unblock_PCB(char* name);


 /**
 * @brief Moves PCB to the suspended state 
 * @param name Name of the process to suspend
 * @returns Status code (0 success, -1 invalid state)
 */
int suspend_PCB(char* name);


 /**
 * @brief Moves PCB to the unsuspended state
 * @param name Name of the process to unsuspend
 * @returns Status code (0 success, -1 invalid state)
 */
int resume_PCB(char* name);


 /**
 * @brief Sets the priority for a PCB
 * @param name Name of the process to alter
 * @param prioritiy The priority that the process will be set to: 0 -> 9 (highest to lowest)
 * @returns Status code (0 success, -1 invalid state)
 */
int set_PCB_priority(char* name, int priority);


 /**
 * @brief Displays a single PCB
 * @param name Name of the process to show
 * @returns Status code (0 success, -1 invalid state)
 */
int show_PCB(char* name);


 /**
 * @brief Displays all PCBs in Ready Queue
 * @returns Status code (0 success, -1 invalid state)
 */
int show_ready_PCB(void);


 /**
 * @brief Displays all PCBs in Blocked Queue
 * @returns Status code (0 success, -1 invalid state)
 */
int show_blocked_PCB(void);
 

 /**
 *  @brief Displays All Processes
 *  @returns Status code (0 success, -1 invalid state)
 */
int show_all_PCB(void);


#endif
