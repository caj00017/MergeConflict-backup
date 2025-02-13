#include <mpx/pcb.h>
#include <memory.h>

/**
 * @file pcb.c 
 * @brief File for pcb struct, getters/setters for fields
 * @authors Chris Jones
 */

pcb* pcb_setup(char* name, int class, int priority) {
    pcb* new_pcb = pcb_allocate();
    new_pcb->name = name;
    new_pcb->class = class;
    new_pcb->priority = priority;
    new_pcb->state = 0;
    new_pcb->execution_mode = 0;
    new_pcb->dispatch_mode = 0;
    new_pcb->stack = NULL;
    new_pcb->stack_location = NULL;
    new_pcb->next_node = NULL;
    new_pcb->prev_node = NULL;
    return new_pcb;
}

queue BLOCKED = {1, NULL, NULL};
queue READY = {0, NULL, NULL};

queue* return_queue(int class) {
    if (class == 0) {
        return &READY;
    } if (class == 1) {
        return &BLOCKED;
    } else {
        return NULL; // invalid class
    }
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

pcb* pcb_find(const char * name){
    //Search all process queues for process with provided name
    return NULL; // return NULL if PCB is not found
}

void pcb_insert(pcb* pcbPtr){
   
    if(pcbPtr == NULL){
        break;
    }
    //Place PCB into appropriate queue based on state and if ready priority ()

    switch (pcbPtr)
    {
        case 0:
            // Ready & Not Suspended
            break;
            
        case 1:
            // Ready & Supended
            break;
        case 2:
            //Running & Not Suspended
            break;
        case 3:
            //Blocked & Not Suspended
            break;

        case 4:
            //Blocked & Suspendid
            break;

        
        default:
            break;
    }

    return;
}


int pcb_remove(pcb* pcbPtr){
    //Remove from queue but don't free memory or structures
    if (pcbPtr == NULL) {
        return 1;
    }

    return 0;
}