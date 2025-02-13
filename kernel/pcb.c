#include <mpx/pcb.h>
#include <memory.h>

/**
 * @file pcb.c 
 * @brief File for pcb struct, getters/setters for fields
 * @authors Chris Jones
 */

char* get_name(pcb* pcbPtr) {
    return pcbPtr.name;
}

int set_name(pcb* pcbPtr, char* name) {
    pcbPtr.name = name;
    return 0;
}

int get_class(pcb* pcbPtr) {
    return pcbPtr.class;
}

int set_class(pcb* pcbPtr, int class) {
    if (class == 0 || class == 1) {
        return -1; // invalid class
    }
    pcbPtr.class = class;
    return 0;
}

int get_priority(pcb* pcbPtr) {
    return pcbPtr.priority;
}

int set_priority(pcb* pcbPtr, int priority) {
    if (priority < 0 || priority > 9) {
        return -1; // invalid priority
    }
    pcbPtr.priority = priority;
    return 0;
}

int get_state(pcb* pcbPtr) {
    return pcbPtr.state;
}

int set_state(pcb* pcbPtr, int state) {
    if (state < 0 || state > 7) {
        return -1; // invalid state
    }
    pcbPtr.state = state;
    return 0;
}

int get_execution_mode(pcb* pcbPtr) {
    return pcbPtr.execution_mode;
}

int set_execution_mode(pcb* pcbPtr, int execution_mode) {
    pcbPtr.execution_mode = execution_mode;
    return 0;
}

int get_dispatch_mode(pcb* pcbPtr) {
    return pcbPtr.dispatch_mode;
}

int set_dispatch_mode(pcb* pcbPtr, int dispatch_mode) {
    pcbPtr.dispatch_mode = dispatch_mode;
    return 0;
}

unsigned char* get_stack_location(pcb* pcbPtr) {
    return pcbPtr.stack_location;
}

int set_stack_location(pcb* pcbPtr, unsigned char* stack_location) {
    pcbPtr.stack_location = stack_location;
    return 0;
}

pcb* get_next_node(pcb* pcbPtr) {
    return pcbPtr.next_node;
}

int set_next_node(pcb* pcbPtr, pcb* next_node) {
    pcbPtr.next_node = next_node;
    return 0;
}

pcb* get_prev_node(pcb* pcbPtr) {
    return pcbPtr.prev_node;
}

int set_prev_node(pcb* pcbPtr, pcb* prev_node) {
    pcbPtr.prev_node = prev_node;
    return 0;
}


pcb* pcb_allocate(void){
    pcb* new_pcb = (pcb*)sys_alloc_mem(sizeof(pcb));
    return new_pcb;
}

int pcb_free(struct pcb* free_pcb){
    if(sys_free_mem(free_pcb)){
        return 0;
    }
    else{
        return 1;
    }
}

