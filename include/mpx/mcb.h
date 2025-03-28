/**
 * @file mcb.h
 * @brief Header file containing MCB-related functions to be implemented in mcb.c.
 * @authors Chris Jones
 */

typedef struct {
    unsigned int start_addr; // base address of usable memory, first byte after MCB
    int size; // size of the block (in bytes)
    mcb* next_node;
    mcb* prev_node;
} mcb;

// CJ - i'm not sure how the list should be defined at the moment
// this struct is based on the queue def. in pcb.h and is subject to change.
typedef struct {
    int list_type; // 0 - free, 1 - allocated.
    mcb* head;
    mcb* tail;
} list;