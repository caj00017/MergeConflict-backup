#include <mpx/queue.h>

char* get_queue_type(queue* queue) {
    return queue.queue_type;
}

int set_queue_type(queue* queue, char* type) {
    if (type == 'r' || type == 'b' || type == 's') {
        return -1; // invalid character
    }
    queue.queue_type = type;
    return 0;
}

queue* get_next(queue* queue) {
    return queue.next;
}

int set_next(queue* queue, queue* next) {
    queue.next = next;
    return 0;
}

pcb* get_next_p(queue* queue) {
    return queue.next_p;
}

int set_next_p(queue* queue, pcb* next_p) {
    queue.next_p = next_p;
    return 0;
}

pcb* get_head(queue* queue) {
    return queue.head;
}

int set_head(queue* queue, pcb* head) {
    queue.head = head;
    return 0;
}

pcb* get_tail(queue* queue) {
    return queue.tail;
}

int set_tail(queue* queue, pcb* tail) {
    queue.tail = tail;
    return 0;
}