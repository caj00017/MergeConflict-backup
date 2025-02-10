#include <pcb.h>

/**
 * @file queue.h 
 * @brief Header file for queue struct, getters/setters for fields
 * @authors Evan Humphrey, Chris Jones
 */

typedef struct {
    char* queue_type;
    queue* next;
    pcb* next_p;
    pcb* head;
    pcb* tail;
} queue;

/**
 * @brief Get the queue type
 * @param queue* The queue to get the type of
 * @returns char* The type of the queue
 */
char* get_queue_type(queue* queue);


/**
 * @brief Get the next queue
 * @param queue* The queue to get the next queue of
 * @returns queue* The next queue in the list
 */
queue* get_next(queue* queue);


/**
 * @brief Get the next pcb in the queue
 * @param queue* The queue to get the next pcb of
 * @returns pcb* The next pcb in the queue
 */
pcb* get_next_p(queue* queue);


/**
 * @brief Get the head of the queue
 * @param queue* The queue to get the head of
 * @returns pcb* The head of the queue
 */
pcb* get_head(queue* queue);


/**
 * @brief Get the tail of the queue
 * @param queue* The queue to get the tail of
 * @returns pcb* The tail of the queue
 */
pcb* get_tail(queue* queue);
