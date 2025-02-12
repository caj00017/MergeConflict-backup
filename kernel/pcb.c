#include <mpx/pcb.h>
#include <memory.h>

/**
 * @file pcb.c 
 * @brief File for pcb struct, getters/setters for fields
 * @authors Chris Jones
 */

char* get_name(pcb* pcb) {
    return pcb.name;
}

int set_name(pcb* pcb, char* name) {
    pcb.name = name;
    return 0;
}

int get_class(pcb* pcb) {
    return pcb.class;
}

int set_class(pcb* pcb, int class) {
    pcb.class = class;
    return 0;
}

int get_priority(pcb* pcb) {
    return pcb.priority;
}

int set_priority(pcb* pcb, int priority) {
    pcb.priority = priority;
    return 0;
}

int get_state(pcb* pcb) {
    return pcb.state;
}

int set_state(pcb* pcb, int state) {
    pcb.state = state;
    return 0;
}

int get_execution_mode(pcb* pcb) {
    return pcb.execution_mode;
}

int set_execution_mode(pcb* pcb, int execution_mode) {
    pcb.execution_mode = execution_mode;
    return 0;
}

int get_dispatch_mode(pcb* pcb) {
    return pcb.dispatch_mode;
}

int set_dispatch_mode(pcb* pcb, int dispatch_mode) {
    pcb.dispatch_mode = dispatch_mode;
    return 0;
}

int get_stack_location(pcb* pcb) {
    return pcb.stack_location;
}

int set_stack_location(pcb* pcb, void* stack_location) {
    pcb.stack_location = stack_location;
    return 0;
}

pcb* get_next_node(pcb* pcb) {
    return pcb.next_node;
}

int set_next_node(pcb* pcb, pcb* next_node) {
    pcb.next_node = next_node;
    return 0;
}

pcb* get_prev_node(pcb* pcb) {
    return pcb.prev_node;
}

int set_prev_node(pcb* pcb, pcb* prev_node) {
    pcb.prev_node = prev_node;
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

