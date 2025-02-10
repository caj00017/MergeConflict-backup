#include <mpx/queue.h>

char* get_queue_type(queue* queue) {
    return queue.queue_type;
}

queue* get_next(queue* queue) {
    return queue.next;
}

pcb* get_next_p(queue* queue) {
    return queue.next_p;
}

pcb* get_head(queue* queue) {
    return queue.head;
}

pcb* get_tail(queue* queue) {
    return queue.tail;
}