#include <mpx/pcb.h>
#include <memory.h>
#include <string.h>

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

int pcb_free(pcb* pcbPtr){
    //check if ptr is null
    if(pcbPtr == NULL){
        return 1;
    }

    //attempt to free memory from Ptr
    if(sys_free_mem(pcbPtr)){
        
        //return 0 on success
        return 0;
    }
    else{

        //return 1 on failure
        return 1;
    }
}

pcb* pcb_find(const char * search_name){

    //getting the ready queue to get the head node of
    queue r_queue = return_queue(0);

    //creating a pcb pointer that begins by pointing to the queue's head
    pcb* current_pcb = r_queue.head;

    //loops while the current pcb pointer isn't null
    while(current_pcb != NULL){

        //checks to see if the name of the current pcb is equal to the given name
        if(strcmp(current_pcb->name, search_name) == 0){

            //if names are equal returns the current_pcb
            return current_pcb;
        }

        //updates the current pcb to the next pcb via it's next_node pointer
        current_pcb == current_pcb->next_node;
    }

    //getting the blocked queue if the searched name isn't found
    queue b_queue = return_queue(1);

    //setting the current pcb to the blocked queue's head
    current_pcb = b_queue.head;

    //loops while current pcb is not null
    while(current_pcb != NULL){

        //checks to see if the names of the current pcb is equal to the given name
        if(strcmp(current_pcb->name, search_name) == 0){

            //if names are equal, returns current pcb pointer
            return current_pcb;
        }

        //updates the current pcb to the next pcb pointer via the next_node pointer
        current_pcb == current_pcb->next_node;
    }

    // return NULL if PCB is not found
    return NULL; 
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


int pcb_remove(pcb* removed_pcbPtr){

    //checkings to make sure the pcb pointer given is not null
    if(removed_pcbPtr == NULL){
        
        //returns 1 if pcb pointer is null
        return 1;
    }

    //checking if the pcb given is at the end of the queue
    if(removed_pcbPtr->next_node == NULL){

        //if at the end, sets the prev_node's next_node pointer to null
        removed_pcbPtr->prev_node->next_node == NULL;
    }

    //checking if the pcb given is at the front of the queue
    else if(removed_pcbPtr->prev_node == NULL){

        //if at the front, sets the next_node's prev_node pointer to null
        removed_pcbPtr->next_node->prev_node == NULL;
    }
    else{

        //sets the prev node's next_node pointer to the removed pcb's next_node pointer
        removed_pcbPtr->prev_node->next_node = removed_pcbPtr->next_node;

        //sets the next node's prev_node pointer to the removed pcb's prev_node pointer
        removed_pcbPtr->next_node->prev_node = removed_pcbPtr->prev_node;
    }

    //sets the removed pcb's next_node and prev_node pointers to null
    removed_pcbPtr->next_node == NULL;
    removed_pcbPtr->prev_node == NULL;

    return 0;
}