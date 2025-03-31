#include <mcb.h>

// allocated and free lists
list ALLOCATED = {1, NULL, NULL};
list FREE = {0, NULL, NULL};
char str[100] = { 0 }; // string buffer for printing with itoa()

mcb* mcb_setup(unsigned int start_addr, int size) {
    mcb* new_mcb = sys_alloc_mem(sizeof(mcb));
    if (new_mcb == NULL) {
        return NULL; // memory allocation failed
    }

    // initialize members
    clear(str);
    new_mcb->start_addr = start_addr;
    print("\nStart address intialized: 0x");
    print(itoa((int)new_mcb->start_addr, str, 10));

    clear(str);
    new_mcb->size = size;
    print("\nSize initialized: ");
    print(itoa(new_mcb->size, str, 10));

    new_mcb->next_node = NULL;
    new_mcb->prev_node = NULL;

    return new_mcb;
}

list* return_list(int list_type) {
    if (list_type == 0) {
        return &FREE;
    } else if (list_type == 1) {
        return &ALLOCATED;
    } else {
        print("Invalid list type\n");
        return NULL; // Invalid list type
    }
}

void mcb_insert(list* list, mcb* new_mcb) {

    /**
     * CJ - Note: the logic for this function is subject to change based on the start address of the MCB. 
     * The current implementation is for testing purposes only. 
     */

    // if list is empty, insert new MCB as head and tail
    if (list->head == NULL) {
        list->head = new_mcb;
        list->tail = new_mcb;

    // otherwise, append new MCB
    } else {
        list->tail->next_node = new_mcb;
        new_mcb->prev_node = list->tail;
        list->tail = new_mcb;
    }

    // regardless, MCB is null
    new_mcb->next_node = NULL;
}

void mcb_remove(list* list, mcb* current_mcb) {

    // if MCB is head
    if (current_mcb == list->head) {

        // the next node is the new head
        list->head = current_mcb->next_node;

        // if list is not empty after removing
        if (list->head != NULL) {
            list->head->prev_node = NULL;
        } 
        
        // if list is empty after removing
        else {
            list->tail = NULL; // list is now empty
        }
    } 
    
    // if MCB is tail
    else if (current_mcb == list->tail) { 

        // the previous node is the new tail
        list->tail = current_mcb->prev_node;

        // if list is not empty after removing
        if (list->tail != NULL) {
            list->tail->next_node = NULL;
        } 
        
        // if list is empty after removing
        else {
            list->head = NULL;
        }
    } 
    
    // MCB is in the middle of the list
    else { 

        // link the previous node to the next node
        current_mcb->prev_node->next_node = current_mcb->next_node;

        // if MCB is not the last node in the list
        if (current_mcb->next_node != NULL) {
            current_mcb->next_node->prev_node = current_mcb->prev_node;
        }
    }

    sys_free_mem(current_mcb); // free the MCB memory
}

mcb* mcb_find(unsigned int start_addr) {

    // search allocated list
    list* allocated_list = return_list(1);
    mcb* current_mcb = allocated_list->head;
    while (current_mcb != NULL) {
        if (current_mcb->start_addr == start_addr) {
            return current_mcb;
        }
        current_mcb = current_mcb->next_node;
    }

    // search free list
    list* free_list = return_list(0);
    current_mcb = free_list->head;
    while (current_mcb != NULL) {
        if (current_mcb->start_addr == start_addr) {
            return current_mcb;
        }
        current_mcb = current_mcb->next_node;
    }

    // MCB not found
    clear(str);
    print("MCB not found: 0x");
    print(itoa((int)start_addr, str, 10));
    return NULL;
}

void show_mcb(mcb* mcb) {
    clear(str);
    print("MCB: 0x");
    print(itoa((int)mcb->start_addr, str, 10));

    clear(str);
    print("\nSize: ");
    print(itoa(mcb->size, str, 10));
    println();
}

void show_alloc_mem(void) {
    list* allocated_list = return_list(1);
    mcb* current_mcb = allocated_list->head;
    while (current_mcb != NULL) {
        print("[ALLOCATED] ");
        show_mcb(current_mcb);
        current_mcb = current_mcb->next_node;
    }
}

void show_free_mem(void) {
    list* free_list = return_list(0);
    mcb* current_mcb = free_list->head;
    while (current_mcb != NULL) {
        print("[FREE] ");
        show_mcb(current_mcb);
        current_mcb = current_mcb->next_node;
    }
}
