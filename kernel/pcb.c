#include <mpx/pcb.h>
#include <memory.h>
#include <string.h>
#include <context.h>

/**
 * @file pcb.c 
 * @brief File for pcb struct, getters/setters for fields
 * @authors Chris Jones
 */

pcb* pcb_setup(char* name, int class, int priority, void (*function_ptr)(void)) {
    pcb* new_pcb = pcb_allocate();
    
    // Allocate memory and copy the name
    new_pcb->name = (char*)sys_alloc_mem(strlen(name) + 1);
    strcpy(new_pcb->name, name);

    // allocate memory for stack
    unsigned char* stack_location = (unsigned char*)sys_alloc_mem(1024); // allocate 1024 bytes for pcb stack

    new_pcb->class = class;
    new_pcb->priority = priority; 
    new_pcb->state = 1;                      // Ready, Not Suspended         
    new_pcb->stack = stack_location;  // beginning of stack, should this jsut be zero?
    new_pcb->stack_ptr = stack_location+1024;     // end of stack
    new_pcb->next_node = NULL;
    new_pcb->prev_node = NULL;


    /* <<<< Creating the Context >>>>  */
    //Allocate memory for the context
    context* ctx = (context*)sys_alloc_mem(sizeof(context));

    // create room for context at the top of the stack
    new_pcb->stack_ptr = new_pcb->stack_ptr - sizeof(ctx); //problem ??

    // create context
    ctx = (context*)new_pcb->stack_ptr;
    

    // initialize code and data registers
    ctx->CS = 0x08;
    ctx->DS = 0x10;
    ctx->ES = 0x10;
    ctx->FS = 0x10;
    ctx->GS = 0x10;
    ctx->SS = 0x10;

    // initialize status control registers
    ctx->EIP = (int) function_ptr; // pointer to what function?
    ctx->EFLAGS = 0x0202;

    // initialize general purpose registers
    ctx->EAX = 0;
    ctx->EBX = 0;
    ctx->ECX = 0;
    ctx->EDX = 0;
    ctx->ESI = 0;
    ctx->EDI = 0;
    ctx->EBP = 0;
    ctx->ESP = 0;

    // store pcb's name in context
    ctx->pcb_name = new_pcb->name;

    //Store context into PCB
    new_pcb->ctx_ptr = ctx;

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

    //creating a pcb pointer that begins by pointing to the queue's head
    pcb* current_pcb = READY.head;

    //loops while the current pcb pointer isn't null
    while(current_pcb != NULL){

        //checks to see if the name of the current pcb is equal to the given name
        if(strcmp(current_pcb->name, search_name) == 0){

            //if names are equal returns the current_pcb
            return current_pcb;
        }

        //updates the current pcb to the next pcb via it's next_node pointer
        current_pcb = current_pcb->next_node;
    }

    //setting the current pcb to the blocked queue's head
    current_pcb = BLOCKED.head;

    //loops while current pcb is not null
    while(current_pcb != NULL){

        //checks to see if the names of the current pcb is equal to the given name
        if(strcmp(current_pcb->name, search_name) == 0){

            //if names are equal, returns current pcb pointer
            return current_pcb;
        }

        //updates the current pcb to the next pcb pointer via the next_node pointer
        current_pcb = current_pcb->next_node;
    }

    // return NULL if PCB is not found
    return NULL; 
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

            if(tempPtr->prev_node != NULL){
                tempPtr->prev_node->next_node = pcbPtr;
                tempPtr->prev_node = pcbPtr;
            }
            else{
                tempPtr->prev_node = pcbPtr;
                READY.head = pcbPtr;
            }
            
            tempPtr = NULL;
            return;
        }
        else{ //pcbPtr-> priority >= 5
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

            if(tempPtr->next_node != NULL){
                tempPtr->next_node->prev_node = pcbPtr;
                tempPtr->next_node = pcbPtr;
            }
            else{
                tempPtr->next_node = pcbPtr;
                READY.tail = pcbPtr;
            }
            
            tempPtr = NULL;
            return;
        }

    }
    /*************************** BLOCKED QUEUE INSERT ******************************/
    else if(pcbPtr->state == BLOCKED_SUS || pcbPtr->state == BLOCKED_NOT_SUS){
        //Place into blocked queue

        //Create pointer to help iterate through queue
        pcb* tempPtr = BLOCKED.head;

        //Case 1: No Nodes in the ready queue
        if(BLOCKED.head == NULL && BLOCKED.tail == NULL){
            //Set both the head and tail equal to the pcb
            BLOCKED.head = pcbPtr;
            BLOCKED.tail = pcbPtr;
 
            return;
        }

        //Case 2: Next Process
        while(tempPtr->next_node != NULL){
            tempPtr = tempPtr->next_node;
        }

        //Add process to end of queue
        tempPtr->next_node = pcbPtr;
        pcbPtr->prev_node = tempPtr;
        BLOCKED.tail = pcbPtr;

        return ; 
        
    }
    else {
        //Write message to notify of currently running
    }


    return;
}





int pcb_remove(pcb* removed_pcbPtr){

    //checkings to make sure the pcb pointer given is not null
    if(removed_pcbPtr == NULL){
        
        //returns 1 if pcb pointer is null
        return 1;
    }

    // checking if this PCB is the only node in the queue
    if (removed_pcbPtr->next_node == NULL && removed_pcbPtr->prev_node == NULL) {

        // checks to see if the given pcb is in the ready queue
        if (removed_pcbPtr->state == READY_SUS || removed_pcbPtr->state == READY_NOT_SUS) {
            READY.head = NULL;
            READY.tail = NULL;
        } 
        
        //checks to see if the given pcb is in the blocked queue
        else if (removed_pcbPtr->state == BLOCKED_SUS || removed_pcbPtr->state == BLOCKED_NOT_SUS) {
            BLOCKED.head = NULL;
            BLOCKED.tail = NULL;
        }

        // sets the removed pcb's next_node and prev_node pointers to null
        removed_pcbPtr->next_node = NULL;
        removed_pcbPtr->prev_node = NULL;
        return 0;
    }
    
    //checking if the pcb given is at the end of the queue
    else if(removed_pcbPtr->next_node == NULL){

        //if at the end, sets the prev_node's next_node pointer to null
        removed_pcbPtr->prev_node->next_node = NULL;

        //checks to see if the given pcb is in the ready queue
        if(removed_pcbPtr->state == READY_SUS || removed_pcbPtr->state == READY_NOT_SUS){

            //if in ready queue replaces the tail of the queue with the previous node of the given pcb
            READY.tail = removed_pcbPtr->prev_node;
        }

        //checks to see if the given pcb is in the blocked queue
        else if(removed_pcbPtr->state == BLOCKED_SUS || removed_pcbPtr->state == BLOCKED_NOT_SUS){

            //if in blocked queue replaces the tail of the queue with the previous node of the given pcb
            BLOCKED.tail = removed_pcbPtr->prev_node;
        }
    }

    //checking if the pcb given is at the front of the queue
    else if(removed_pcbPtr->prev_node == NULL){

        //if at the front, sets the next_node's prev_node pointer to null
        removed_pcbPtr->next_node->prev_node = NULL;

        //checks to see if the given pcb is in the ready queue
        if(removed_pcbPtr->state == READY_SUS || removed_pcbPtr->state == READY_NOT_SUS){

            //if in ready queue replaces the head of the queue with the next node of the given pcb
            READY.head = removed_pcbPtr->next_node;
        }

        //checks to see if the given pcb is in the blocked queue
        else if(removed_pcbPtr->state == BLOCKED_SUS || removed_pcbPtr->state == BLOCKED_NOT_SUS){

             //if in blocked queue replaces the head of the queue with the next node of the given pcb
            BLOCKED.head = removed_pcbPtr->next_node;
        }
    }
    else{

        //sets the prev node's next_node pointer to the removed pcb's next_node pointer
        removed_pcbPtr->prev_node->next_node = removed_pcbPtr->next_node;

        //sets the next node's prev_node pointer to the removed pcb's prev_node pointer
        removed_pcbPtr->next_node->prev_node = removed_pcbPtr->prev_node;
    }


    //sets the removed pcb's next_node and prev_node pointers to null
    removed_pcbPtr->next_node = NULL;
    removed_pcbPtr->prev_node = NULL;
    

    return 0;
}
