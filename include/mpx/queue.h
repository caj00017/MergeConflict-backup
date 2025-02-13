#include <pcb.h>

/**
 * @file queue.h 
 * @brief Header file for queue struct, getters/setters for fields
 * @authors Evan Humphrey, Chris Jones
 */

typedef struct queue {
    char* queue_type;
    queue* next;

} queue;

