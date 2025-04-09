#include <mcb.h>

list ALLOCATED = {1, NULL, NULL};
list FREE = {0, NULL, NULL};
char str[100] = { 0 };

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
    new_mcb->start_addr = start_addr + sizeof(mcb);
    print("\nStart address intialized: 0x");
    print(custom_itoa((int)new_mcb->start_addr, str, 16));

    clear(str);
    new_mcb->size = size;
    print("\nSize initialized: ");
    print(custom_itoa(new_mcb->size, str, 10));

    new_mcb->next_node = NULL;
    new_mcb->prev_node = NULL;
    new_mcb->status = 0; // 0 - free, 1 - allocated

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

    //Possibly change mcb size to size_t????

    
    // Check if Free has anything
    if(FREE.head == NULL || FREE.head->size == 0){
        print_error("There was not enough free memory to allocate.");
        return NULL;
    }


    //Pointer to find best fit and temp pointer to iterate through list->
    mcb* bestFitPtr = NULL;
    mcb* tempPtr = FREE.head;

    //Iterate through to find the MCB with the best size compared to the desired size.
    while(tempPtr != NULL){
        if (bestFitPtr == NULL  &&  tempPtr->size >= (int) size){
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
    unsigned int start_address_alloc = bestFitPtr->start_addr - sizeof(mcb);

    //Insert the allocated memory block into the proper list-> 
    mcb * allocPtr = mcb_setup( start_address_alloc , size );
    mcb_insert( &ALLOCATED , allocPtr); // Allocated Block

    //Check if it was a perfect fit
    if( start_address_free != 0){
        //Place the reduced free memory block back in
        mcb_insert( &FREE , mcb_setup( start_address_free,  (bestFitPtr->size - size) )); //Smaller Free Block
    }   
    
    bestFitPtr = NULL;

    //Return address of block not mcb
    return (void *) allocPtr->start_addr;

    //Make sure memory is not being over written.
}

int free_memory(void *addr){
    
    // search allocated list
    list* allocated_list = return_list(1);
    mcb* current_mcb = allocated_list->head;
    while (current_mcb != NULL) {
        if (current_mcb->start_addr == (unsigned int)addr) {
            break;
        }
        current_mcb = current_mcb->next_node;
    }

    if(current_mcb == NULL){
        return 1;
    }

    mcb_remove(&ALLOCATED, current_mcb);
    mcb_insert(&FREE, current_mcb);

    return 0;


    return 0;
}


void mcb_insert(list* list, mcb* new_mcb) {


    // if list is empty, insert new MCB as head and tail
    if (list->head == NULL) {
        list->head = new_mcb;
        list->tail = new_mcb;
        return ;
    } 


    /* ADD NEW MCB TO THE LIST*/
    
    //Create temp pointer to iterate through 
    mcb* tempPtr = list->head;
    mcb * combined_mcb;

    /*CHECK BEFORE CURRENT*/
    // Since we know there is one at the head, check to see if the new one goes before it.
    if(new_mcb->start_addr < tempPtr->start_addr){
      
        //Check if they combine (only on Free list)
        if( (new_mcb->start_addr + new_mcb->size + sizeof(mcb) == tempPtr->start_addr )   && (list->head == FREE.head) ){
            //Combine the new and current mcbs.
            combined_mcb = mcb_setup(new_mcb->start_addr- sizeof(mcb), (tempPtr->size + new_mcb->size) ); 

            //Attach combined 
            combined_mcb->next_node = tempPtr;
            
            if(tempPtr->prev_node == NULL){
                combined_mcb->prev_node = NULL;
                FREE.head = combined_mcb;
            }
            else{
                combined_mcb->prev_node = tempPtr->prev_node;
                tempPtr-> prev_node->next_node = combined_mcb;
            }

            tempPtr->prev_node = combined_mcb;

            //Remove the temp mcb
            mcb_remove(&FREE, tempPtr); 
            tempPtr = NULL;

            return;

        }
        else{
            //Place before current but don't combine
            new_mcb->next_node = tempPtr;

            //Check if previous is NULL
            if(tempPtr->prev_node == NULL){
                new_mcb->prev_node = NULL;
                list->head = new_mcb;
            }
            else{
                tempPtr->prev_node->next_node = new_mcb;
                new_mcb->prev_node = tempPtr->prev_node;
            }
            
            tempPtr->prev_node = new_mcb;

            tempPtr = NULL;
            return;
        }
    }

    while(tempPtr != NULL){
        
        /* CHECK IF THERE IS A NEXT MCB*/
        if(tempPtr->next_node == NULL){
            // The temp node is the last one in the list

            //Check to see if the last one can be combined
            if (( (tempPtr->start_addr + tempPtr->size + sizeof(mcb)) == new_mcb->start_addr)  && FREE.head == list->head){
                    //Create combined mcb
                    mcb * combined_mcb = mcb_setup(tempPtr->start_addr - sizeof(mcb), (tempPtr->size + new_mcb->size) ); 

                    //Add combined memory block to tail
                    tempPtr->next_node = combined_mcb;
                    combined_mcb->prev_node = tempPtr;
                    
                    list->tail = combined_mcb;

                    // remove temp
                    mcb_remove(list,tempPtr);

                }
                 // Otherwise place at the end of list
                else{
                    tempPtr->next_node = new_mcb;
                    new_mcb->prev_node = tempPtr;

                    list->tail = new_mcb;
                }  


            return;
        }


        /*CHECK BETWEEN CURRENT AND NEXT*/

        //Since it was not found to be before the current, check between the current and the next.
        if (  (new_mcb->start_addr >= tempPtr->start_addr)  &&  (new_mcb->start_addr < tempPtr->next_node->start_addr) ) {
        
            /* CHECK TO SEE IF MEMORY BLOCKS COMBINE (Only for FREE list)*/

            //check if both blocks match
            if (  (tempPtr->start_addr + tempPtr->size + sizeof(mcb)) == new_mcb->start_addr && 
                    ( (new_mcb->start_addr + new_mcb->size + sizeof(mcb)) == tempPtr->next_node->start_addr) &&
                    (list->head == FREE.head) ){// Perfect Fit!!!
                //Combining next and new
                combined_mcb = mcb_setup(tempPtr->start_addr - sizeof(mcb), (tempPtr->size + tempPtr->next_node->size + new_mcb->size) ); 
                

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
            // Check if it matches previous block
            else if(( (tempPtr->start_addr + tempPtr->size + sizeof(mcb)) == new_mcb->start_addr)  && (list->head == FREE.head)) {  
                //Combining the temp and new
                combined_mcb = mcb_setup(tempPtr->start_addr - sizeof(mcb), (tempPtr->size + new_mcb->size) ); 
                
                //Attach combined to next
                combined_mcb->next_node = tempPtr->next_node; 
                tempPtr->next_node->prev_node = combined_mcb; 

                //Attach combined to previous 
                combined_mcb->prev_node =  tempPtr; 
                tempPtr->next_node = combined_mcb;

                mcb_remove(&FREE, tempPtr);  //Remove the uncombined section

                return;

            }
            // check if it matches next block
            else if(( (new_mcb->start_addr + new_mcb->size + sizeof(mcb)) == tempPtr->next_node->start_addr)  && (list->head == FREE.head )) {
                //Combining next and new
                combined_mcb = mcb_setup(new_mcb->start_addr - sizeof(mcb), (tempPtr->next_node->size + new_mcb->size) );
                
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
            else{ // No matches or not in free list
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

    // It didn't find a spot anywhere in the list for it. Only happens if stack_addr is messed up. (guessing)
    print_error("ERROR INSERTING OF MCB");
    return;

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
    print(custom_itoa((int)start_addr, str, 16));
    return NULL;
}

