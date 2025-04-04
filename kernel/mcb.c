#include <mcb.h>
#include <mpx/vm.h>

// allocated and free lists
list ALLOCATED = {1, NULL, NULL};
list FREE = {0, NULL, NULL};
char str[100] = { 0 }; // string buffer for printing with itoa()

void initialize_heap(size_t size) {
    void* heap = kmalloc(size, 0, NULL); //allocate singular page for heap
    if(heap == NULL) {
        print("Heap allocation failed\n");
        return;
    }

    //i noticed that there's no flag to check if the mcb is related to a free or alloced block
    mcb* new_mcb = mcb_setup((unsigned int)heap, (int)size);


    mcb_insert(return_list(0), new_mcb); //set the head of the free list to the new MCB

    //initialize allocated list to be empty
    //maybe not necessary?
    ALLOCATED.head = NULL;

}

mcb* mcb_setup(unsigned int start_addr, int size) {
    mcb* new_mcb = sys_alloc_mem(sizeof(mcb));
    if (new_mcb == NULL) {
        return NULL; // memory allocation failed
    }

    // initialize members
    clear(str);
    new_mcb->start_addr = start_addr;
    print("\nStart address intialized: 0x");
    print(itoa((int)new_mcb->start_addr, str, 10));

    clear(str);
    new_mcb->size = size;
    print("\nSize initialized: ");
    print(itoa(new_mcb->size, str, 10));

    new_mcb->next_node = NULL;
    new_mcb->prev_node = NULL;

    return new_mcb;
}

list* return_list(int list_type) {
    if (list_type == 0) {
        return &FREE;
    } else if (list_type == 1) {
        return &ALLOCATED;
    } else {
        print("Invalid list type\n");
        return NULL; // Invalid list type
    }
}


void * allocate_memory(size_t size){

    //Can we get rid of tail???
    //Possibly change mcb size to size_t????

    //No point in allocating zero bytes or fewer
    if(size <= 0){
        return NULL;
    }

    // Check if Free has anything
    if(FREE.head == NULL){
        return NULL;
    }

    //Pointer to find best fit and temp pointer to iterate through list->
    mcb* bestFitPtr = NULL;
    mcb* tempPtr = FREE.head;

    //Iterate through to find the MCB with the best size compared to the desired size.
    while(tempPtr != NULL){
        if (bestFitPtr == NULL &&  tempPtr->size >= (int) size){
            bestFitPtr = tempPtr;
        }
        else if (tempPtr->size < bestFitPtr->size &&  tempPtr->size >= (int) size){
            bestFitPtr = tempPtr;
        }
        
        tempPtr = tempPtr->next_node;
    }

    //Check if the best fit has a value
    if(bestFitPtr == NULL){
        //There was no mcb big enough
        print_error("There was not enough memory to allocate.");
        return NULL;
    }

    //Remove the original free block
    mcb_remove(&FREE, bestFitPtr);

    //Get the new address for the smaller block
    unsigned int start_address_free = bestFitPtr->start_addr + size; //Need to check
    unsigned int start_address_alloc = bestFitPtr->start_addr;

    //Insert the allocated memory block into the proper list-> 
    mcb * allocPtr = mcb_setup(  start_address_alloc , size );
    mcb_insert( &ALLOCATED , allocPtr); // Allocated Block

    //Check if it was a perfect fit
    if( start_address_free != 0){
        //Place the reduced free memory block back in
        mcb_insert( &FREE , mcb_setup( start_address_free,  (bestFitPtr->size - size) )); //Smaller Free Block
    }   
    
    //Return address of block not mcb
    return (void *) allocPtr->start_addr;

    //Make sure memory is not being over written.
}


void mcb_insert(list* list, mcb* new_mcb) {

    /**
     * CJ - Note: the logic for this function is subject to change based on the start address of the MCB. 
     * The current implementation is for testing purposes only. 
     * 
     * IW - Wrote a bunch of stuff and have not tested. Going to look into testing tomorrow.
     * !!!!!!!!!!!!!!!!!Write one for when its inserts before the first pointer!!!!!!!!!!!!!!
     */

    // if list is empty, insert new MCB as head and tail
    if (list->head == NULL) {
        list->head = new_mcb;
        list->tail = new_mcb;
        return ;
    
    } 
    //Append New MCB
    else {
        //Create temp pointer to iterate through 
        mcb* tempPtr = list->head;

        while(tempPtr->next_node != NULL){
            //Check to see if its position is found

            
            //check to see if it goes between this node and the next
            if (  (new_mcb->start_addr >= tempPtr->start_addr)  &&  (new_mcb->start_addr < tempPtr->next_node->start_addr) ) {
                mcb * combined_mcb;

                // Check to see if we can attach it to the surrounding memory blocks( ONLY DO THIS FOR FREE LIST!  )
                //I am not sure how the memory addressing works. Skipping for now.                                              !!!!!!!!!!!!!!!!!!!!!!!!

                if(0 && (list->head == FREE.head)) {  // Check if it matches previous block
                    //Combining the temp and new
                    combined_mcb = mcb_setup(tempPtr->start_addr, (tempPtr->size + new_mcb->size) ); 
                     
                    //Attach combined to next
                    combined_mcb->next_node = tempPtr->next_node; 
                    tempPtr->next_node->prev_node = combined_mcb; 

                    //Attach combined to previous (override)
                    combined_mcb->prev_node =  tempPtr; 
                    tempPtr->next_node = combined_mcb;

                    mcb_remove(&FREE, tempPtr);  //Remove the uncombined section

                    return;

                }
                else if(0 && (list->head == FREE.head )) {// check if it matches next block
                    //Combining next and new
                    combined_mcb = mcb_setup(new_mcb->start_addr, (tempPtr->next_node->size + new_mcb->size) );
                    
                    //Attach combined to next
                    combined_mcb->next_node = tempPtr->next_node; 
                    tempPtr->next_node->prev_node = combined_mcb; 

                    //Attach combined to previous (override)
                    combined_mcb->prev_node =  tempPtr; 
                    tempPtr->next_node = combined_mcb; 
                    
                    //Remove the next node
                    mcb_remove(&FREE,combined_mcb->next_node);

                    tempPtr = NULL;
                    return;
                    
                }
                else if (0 && (list->head == FREE.head)) {// Perfect Fit!!!
                    //Combining next and new
                    combined_mcb = mcb_setup(tempPtr->start_addr, (tempPtr->size + tempPtr->next_node->size + new_mcb->size) ); 
                    

                    //Attach combined to next
                    combined_mcb->next_node = tempPtr->next_node; 
                    tempPtr->next_node->prev_node = combined_mcb; 

                    //Attach combined to previous (override)
                    combined_mcb->prev_node =  tempPtr; 
                    tempPtr->next_node = combined_mcb; 
                    
                    //Remove the next node and previous nodes
                    mcb_remove(&FREE, combined_mcb->next_node);
                    mcb_remove(&FREE, tempPtr);


                    tempPtr =NULL;

                    return;
                }
                else{ // No matches
                    //Correct spot, doesn't pair with the next door blocks.

                    new_mcb->next_node = tempPtr->next_node;
                    tempPtr->next_node->prev_node = new_mcb;

                    new_mcb->prev_node = tempPtr;
                    tempPtr->next_node = new_mcb;

                    tempPtr = NULL;
                    return;

                }


            }

            // Not it the correct position. Continue to next location.
            tempPtr = tempPtr->next_node;

        }

        /* <LAST MEMORY BLOCK>*/
        // Check if it can be combined with preivous
        if (0 && FREE.head == list->head){
            mcb * combined_mcb = mcb_setup(tempPtr->start_addr, (tempPtr->size + new_mcb->size) ); 

            //Add combined memory block to tail
            tempPtr->next_node = combined_mcb;
            combined_mcb->prev_node = tempPtr;
            
            list->tail = combined_mcb;

            // remove temp
            mcb_remove(list,tempPtr);

        }
        else{
            //Not combinable, still last memory block in list
            tempPtr->next_node = new_mcb;
            new_mcb->prev_node = tempPtr;

            list->tail = new_mcb;
        }

    }

}

void mcb_remove(list* list, mcb* current_mcb) {

    if (current_mcb == NULL) {
        print("MCB is NULL\n");
        return; // nothing to remove
    }

    // if MCB is head
    if (current_mcb == list->head) {

        // the next node is the new head
        list->head = current_mcb->next_node;

        // if list is not empty after removing
        if (list->head != NULL) {
            list->head->prev_node = NULL;
        } 
        
        // if list is empty after removing
        else {
            list->tail = NULL; // list is now empty
        }
    } 
    
    // if MCB is tail
    else if (current_mcb == list->tail) { 

        // the previous node is the new tail
        list->tail = current_mcb->prev_node;

        // if list is not empty after removing
        if (list->tail != NULL) {
            list->tail->next_node = NULL;
        } 
        
        // if list is empty after removing
        else {
            list->head = NULL;
        }
    } 
    
    // MCB is in the middle of the list
    else { 

        // ensure MCB is indeed not the head or tail
        if (current_mcb->prev_node == NULL || current_mcb->next_node == NULL) {
            return;
        }        

        // link the previous node to the next node
        current_mcb->prev_node->next_node = current_mcb->next_node;

        // if MCB is not the last node in the list
        if (current_mcb->next_node != NULL) {
            current_mcb->next_node->prev_node = current_mcb->prev_node;
        }
    }

    // sys_free_mem(current_mcb); // free the MCB memory
}

mcb* mcb_find(unsigned int start_addr) {

    // search allocated list
    list* allocated_list = return_list(1);
    mcb* current_mcb = allocated_list->head;
    while (current_mcb != NULL) {
        if (current_mcb->start_addr == start_addr) {
            return current_mcb;
        }
        current_mcb = current_mcb->next_node;
    }

    // search free list
    list* free_list = return_list(0);
    current_mcb = free_list->head;
    while (current_mcb != NULL) {
        if (current_mcb->start_addr == start_addr) {
            return current_mcb;
        }
        current_mcb = current_mcb->next_node;
    }

    // MCB not found
    clear(str);
    print("MCB not found: 0x");
    print(itoa((int)start_addr, str, 10));
    return NULL;
}

void show_mcb(mcb* mcb) {
    clear(str);
    print("MCB: 0x");
    print(itoa((int)mcb->start_addr, str, 10));

    clear(str);
    print("\nSize: ");
    print(itoa(mcb->size, str, 10));
    println();
}

void show_alloc_mem(void) {
    list* allocated_list = return_list(1);
    mcb* current_mcb = allocated_list->head;
    while (current_mcb != NULL) {
        print("[ALLOCATED] ");
        show_mcb(current_mcb);
        current_mcb = current_mcb->next_node;
    }
}

void show_free_mem(void) {
    list* free_list = return_list(0);
    mcb* current_mcb = free_list->head;
    while (current_mcb != NULL) {
        print("[FREE] ");
        show_mcb(current_mcb);
        current_mcb = current_mcb->next_node;
    }
}

