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
    new_pcb->state = 0;               //Change?
    new_pcb->stack = NULL;             // Should we allocate here?
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
    if(name == 0){
        return NULL;
    }
    //Search all process queues for process with provided name
    return NULL; // return NULL if PCB is not found
}

void pcb_insert(pcb* pcbPtr){
   //Check if we are given a valid pcbPtr
    if(pcbPtr == NULL){
        return;
    }

    //Spilt up by state

    /*************************** READY QUEUE INSERT ******************************/
    if(pcbPtr->state == READY_SUS || pcbPtr->state == READY_NOT_SUS ){  
        //Create pointer to help iterate through array
        pcb* tempPtr = NULL;

        //Case 1: No Nodes in the ready queue
        if(READY.head == NULL && READY.tail == NULL){
            //Set both the head and tail equal to the pcb
            READY.head = pcbPtr;
            READY.tail = pcbPtr;

            return;
        }

        //Case 2: Next item is either before or after halfway point
        if(READY.head->priority >= 5 && pcbPtr->priority < 5){
            //Adjust first node
            READY.head->prev_node = pcbPtr;
            pcbPtr->next_node = READY.head;
            READY.head = pcbPtr;
            return;
        }
        else if (READY.tail->priority < 5 && pcbPtr->priority >= 5 ){
            //Adjust last node
            READY.tail->next_node = pcbPtr;
            pcbPtr->prev_node = READY.tail;
            READY.tail = pcbPtr;
            return;
        }


        //Place into ready queue using head and tail to make sorting easier.
        if(pcbPtr->priority < 5){
            //Place into top of queue with FIFO based on each priority level
            tempPtr = READY.head;

            //Shift the position of the temp pointer until we are that the one we want to append.
            while(pcbPtr->priority >= tempPtr->priority){
                if(tempPtr->next_node == NULL){
                    //Reached end, add node
                    tempPtr->next_node = pcbPtr;
                    pcbPtr->prev_node = tempPtr;
                    READY.tail = pcbPtr;
                    tempPtr = NULL;
                    return;
                }
                tempPtr = tempPtr->next_node;
            }

            //Proper place has been located, move pointers to insert item
            pcbPtr->next_node = tempPtr;
            pcbPtr->prev_node = tempPtr->prev_node;

            tempPtr->prev_node->next_node = pcbPtr;
            tempPtr->prev_node = pcbPtr;

            tempPtr = NULL;
            return;
        }
        else{
            //Place into bottom of queue with FIFO based on each priority level
            tempPtr = READY.tail;

            //Shift the position of the temp pointer until we are that the one we want to append.
            while(pcbPtr->priority < tempPtr->priority){
                
                if(tempPtr->prev_node == NULL){
                    //Reached start, add node.
                    tempPtr->prev_node = pcbPtr;
                    pcbPtr->next_node = tempPtr;
                    tempPtr = NULL;
                    READY.head = pcbPtr;
                    return;
                }
                tempPtr = tempPtr->prev_node;
            }

            //Proper place has been located, move pointers to insert item
            pcbPtr->next_node = tempPtr->next_node;
            pcbPtr->prev_node = tempPtr;

            tempPtr->next_node->prev_node = pcbPtr;
            tempPtr->next_node = pcbPtr;

            tempPtr = NULL;
            return;
        }

    }
    /*************************** BLOCKED QUEUE INSERT ******************************/
    else if(pcbPtr->state == BLOCKED_SUS || pcbPtr->state == BLOCKED_NOT_SUS){
        //Place into blocked queue

         //Create pointer to help iterate through queue
         pcb* tempPtr = NULL;

         //Case 1: No Nodes in the ready queue
         if(BLOCKED.head == NULL && BLOCKED.tail == NULL){
             //Set both the head and tail equal to the pcb
             BLOCKED.head = pcbPtr;
             BLOCKED.tail = pcbPtr;
 
             return;
         }
 
         //Case 2: Next item is either before or after halfway point
         if(BLOCKED.head->priority >= 5 && pcbPtr->priority < 5){
             //Adjust first node
             BLOCKED.head->prev_node = pcbPtr;
             pcbPtr->next_node = BLOCKED.head;
             BLOCKED.head = pcbPtr;
             return;
         }
         else if (BLOCKED.tail->priority < 5 && pcbPtr->priority >= 5 ){
             //Adjust last node
             BLOCKED.tail->next_node = pcbPtr;
             pcbPtr->prev_node = BLOCKED.tail;
             BLOCKED.tail = pcbPtr;
             return;
         }
 
 
         //Place into ready queue using head and tail to make sorting easier.
         if(pcbPtr->priority < 5){
             //Place into top of queue with FIFO based on each priority level
             tempPtr = BLOCKED.head;
 
             //Shift the position of the temp pointer until we are that the one we want to append.
             while(pcbPtr->priority >= tempPtr->priority){
                 if(tempPtr->next_node == NULL){
                     //Reached end, add node
                     tempPtr->next_node = pcbPtr;
                     pcbPtr->prev_node = tempPtr;
                     BLOCKED.tail = pcbPtr;
                     tempPtr = NULL;
                     return;
                 }
                 tempPtr = tempPtr->next_node;
             }
 
             //Proper place has been located, move pointers to insert item
             pcbPtr->next_node = tempPtr;
             pcbPtr->prev_node = tempPtr->prev_node;
 
             tempPtr->prev_node->next_node = pcbPtr;
             tempPtr->prev_node = pcbPtr;
 
             tempPtr = NULL;
             return;
         }
         else{
             //Place into bottom of queue with FIFO based on each priority level
             tempPtr = BLOCKED.tail;
 
             //Shift the position of the temp pointer until we are that the one we want to append.
             while(pcbPtr->priority < tempPtr->priority){
                 
                 if(tempPtr->prev_node == NULL){
                     //Reached start, add node.
                     tempPtr->prev_node = pcbPtr;
                     pcbPtr->next_node = tempPtr;
                     tempPtr = NULL;
                     BLOCKED.head = pcbPtr;
                     return;
                 }
                 tempPtr = tempPtr->prev_node;
             }
 
             //Proper place has been located, move pointers to insert item
             pcbPtr->next_node = tempPtr->next_node;
             pcbPtr->prev_node = tempPtr;
 
             tempPtr->next_node->prev_node = pcbPtr;
             tempPtr->next_node = pcbPtr;
 
             tempPtr = NULL;
             return;
         }
 

        
    }
    else {
        //Write message to notify of currently running
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
