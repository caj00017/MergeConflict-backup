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

/**
 * @brief Get the queue type
 * @param queue* The queue to get the type of
 * @returns char* The type of the queue
 */
char* get_queue_type(queue* queue);

/**
 * @brief Set the queue type
 * @param queue* The queue to set the type of
 * @param char* The type to set (r)eady, (b)locked, (s)uspended
 * @returns Status code (0 success, -1 failure)
 */
int set_queue_type(queue* queue, char* type);

/**
 * @brief Get the next queue
 * @param queue* The queue to get the next queue of
 * @returns queue* The next queue in the list
 */
queue* get_next(queue* queue);

/**
 * @brief Set the next queue
 * @param queue* The queue to set the next queue of
 * @param queue* The next queue to set
 * @returns Status code
 */
int set_next(queue* queue, queue* next);

/**
 * @brief Get the next pcb in the queue
 * @param queue* The queue to get the next pcb of
 * @returns pcb* The next pcb in the queue
 */
pcb* get_next_p(queue* queue);

/**
 * @brief Set the next pcb in the queue
 * @param queue* The queue to set the next pcb of
 * @param pcb* The next pcb to set
 * @returns Status code
 */
int set_next_p(queue* queue, pcb* next_p);

/**
 * @brief Get the head of the queue
 * @param queue* The queue to get the head of
 * @returns pcb* The head of the queue
 */
pcb* get_head(queue* queue);

/**
 * @brief Set the head of the queue
 * @param queue* The queue to set the head of
 * @param pcb* The head to set
 * @returns Status code
 */
int set_head(queue* queue, pcb* head);

/**
 * @brief Get the tail of the queue
 * @param queue* The queue to get the tail of
 * @returns pcb* The tail of the queue
 */
pcb* get_tail(queue* queue);

/**
 * @brief Set the tail of the queue
 * @param queue* The queue to set the tail of
 * @param pcb* The tail to set
 * @returns Status code
 */
int set_tail(queue* queue, pcb* tail);
