#include <string.h>
#include <stdlib.h>
#include <memory.h>

/**
 * @file mcb.h
 * @brief Header file containing MCB-related functions to be implemented in mcb.c.
 * @authors Chris Jones
 */

 typedef struct mcb {
    unsigned int start_addr; // base address of usable memory, first byte after MCB
    int size; // size of the block (in bytes)
    struct mcb* next_node;
    struct mcb* prev_node;
    int status; // 0 - free, 1 - allocated
} mcb;

// CJ - i'm not sure how the list should be defined at the moment
// this struct is based on the queue def. in pcb.h and is subject to change.
typedef struct {
    int list_type; // 0 - free, 1 - allocated.
    mcb* head;
    mcb* tail;
} list;

/**
 * @brief Setup a new MCB with the given start address and size.
 * @param start_addr The starting address of the MCB.
 * @param size The size of the MCB.
 * @return A pointer to the newly created MCB.
 * @author Chris Jones
 */
mcb* mcb_setup(unsigned int start_addr, int size);

/**
 * @brief Return the list based on the list type.
 * @param list_type The type of the list (0 for free, 1 for allocated).
 * @return A pointer to the requested list.
 * @author Chris Jones
 */
list* return_list(int list_type);

/**
 * @brief Insert a new MCB into the specified list.
 * @param list The list to insert the MCB into.
 * @param new_mcb The MCB to insert.
 * @author Chris Jones
 */
void mcb_insert(list* list, mcb* new_mcb);

/**
 * @brief Remove an MCB from the specified list.
 * @param list The list to remove the MCB from.
 * @param current_mcb The MCB to remove.
 * @author Chris Jones
 */
void mcb_remove(list* list, mcb* current_mcb);

/**
 * @brief Find an MCB in the specified list based on the start address.
 * @param start_addr The starting address of the MCB to find.
 * @return A pointer to the found MCB, or NULL if not found.
 * @author Chris Jones
 */
mcb* mcb_find(unsigned int start_addr);

/**
 * @brief Show the details of a specific MCB.
 * @param mcb The MCB to show.
 * @author Chris Jones
 */
void show_mcb(mcb* mcb);

/**
 * @brief Show the allocated memory blocks.
 * @author Chris Jones
 */
void show_alloc_mem(void);

/**
 * @brief Show the free memory blocks.
 * @author Chris Jones
 */
void show_free_mem(void);
