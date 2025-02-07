#include <mpx/pcb.h>

/**
 * @file pcb.c 
 * @brief File for pcb struct, getters/setters for fields
 * @authors Chris Jones
 */

char* get_name(pcb* pcb) {
    return pcb.name;
}

int get_class(pcb* pcb) {
    return pcb.class;
}

int get_priority(pcb* pcb) {
    return pcb.priority;
}

int get_state(pcb* pcb) {
    return pcb.state;
}

int get_execution_mode(pcb* pcb) {
    return pcb.execution_mode;
}

int get_dispatch_mode(pcb* pcb) {
    return pcb.dispatch_mode;
}

int get_stack_location(pcb* pcb) {
    return pcb.stack_location;
}

pcb* get_next_node(pcb* pcb) {
    return pcb.next_node;
}

pcb* get_prev_node(pcb* pcb) {
    return pcb.prev_node;
}

