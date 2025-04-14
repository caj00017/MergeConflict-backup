#include <mcb.h>

#define MCB_ARR_SIZE 4096

char mcb_arr[MCB_ARR_SIZE];
int mcb_offset = 0;

list MCB_LIST = {NULL, NULL};
char str[100] = { 0 };

void initialize_heap(size_t size) {
    void* heap = kmalloc(size, 0, NULL); //allocate singular page for heap
    if(heap == NULL) {
        print("Heap allocation failed\n");
        return;
    }

    //i noticed that there's no flag to check if the mcb is related to a free or alloced block
    mcb* new_mcb = mcb_setup((unsigned int)heap, (int)size, "HEAP");
    new_mcb->status = 0; // Set status to free

    mcb_insert(new_mcb); //set the head of the list to the new MCB

}

mcb* mcb_setup(unsigned int start_addr, int size, char* name) {
    mcb* new_mcb = alloc_mcb();
    if (new_mcb == NULL) {
        return NULL; // memory allocation failed
    }

    // initialize members
    clear(str);
    new_mcb->start_addr = start_addr + sizeof(mcb);
    print("\nStart address intialized: 0x");
    print(custom_itoa((int)new_mcb->start_addr, str, 16));

    clear(str);
    new_mcb->size = size - sizeof(mcb);
    print("\nSize initialized: ");
    print(custom_itoa((new_mcb->size+20), str, 10));

    new_mcb->name = name;
    new_mcb->next_node = NULL;
    new_mcb->prev_node = NULL;

    return new_mcb;
}


mcb* alloc_mcb(void){
    if(mcb_offset + sizeof(mcb) > MCB_ARR_SIZE){
        return NULL;
    }

    mcb* new_mcb = (mcb*)(mcb_arr + mcb_offset);
    mcb_offset += sizeof(mcb);
    return new_mcb;
}

list* return_list() {
    return &MCB_LIST;
}


void * allocate_memory(size_t size, char* name){

    //Check if size is an appropriate value
    if(size<=0){
        print_error("Passed size cannot be allocated.\n");
        return NULL;
    }

    /*CHECK IF THERE ARE AVAILABLE FREE BLOCKS*/
    if(MCB_LIST.head == NULL){
        print_error("Cannot Allocate Memory ; No Blocks Available ");
    }

    //Pointer to find best fit and temp pointer to iterate through list
    mcb* bestFitPtr = NULL;
    mcb* tempPtr = MCB_LIST.head;

    //Iterate through to find the MCB with the best size compared to the desired size.
    while(tempPtr != NULL){
        if (bestFitPtr == NULL && tempPtr->status == 0 && (tempPtr->size >= (int) size) ){
            bestFitPtr = tempPtr;
        }
        else if (bestFitPtr != NULL && tempPtr->status == 0 && (tempPtr->size >= (int) size)  && (tempPtr->size < bestFitPtr->size) ){
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
    mcb_remove(bestFitPtr);

    //Get the new address for the smaller block
    unsigned int start_address_alloc = bestFitPtr->start_addr - sizeof(mcb);
    unsigned int start_address_free = bestFitPtr->start_addr + size; //Need to check
    

    //Insert the allocated memory block into the proper list-> 
    mcb * allocPtr = mcb_setup( start_address_alloc , size, name );
    allocPtr->status = 1; // Change status to allocated
    mcb_insert(allocPtr); // Allocated Block

    //Check if it was a perfect fit
    if(start_address_free != 0){
        //Place the reduced free memory block back in
        mcb * freePtr = mcb_setup( start_address_free,  (bestFitPtr->size - size + sizeof(mcb)), name );
        freePtr->status = 0;
        mcb_insert(freePtr); //Smaller Free Block
    }   

    bestFitPtr = NULL;
    
    // update the status of the allocated block
    allocPtr->status = 1; // 1 - allocated

    //Return address of block not mcb
    return (void *) allocPtr->start_addr;

}

int free_memory(void *addr){
    
    // search list for memory address
    mcb* current_mcb = MCB_LIST.head;

    while (current_mcb != NULL) {
        if (current_mcb->start_addr == (unsigned int)addr) {
            break;
        }
        current_mcb = current_mcb->next_node;
    }
    //Check if there was a node found
    if(current_mcb == NULL){
        return 1;
    }

    current_mcb->status = 0; // Set flag to FREE


    /* Check if combinable with next node*/
    if(current_mcb->next_node != NULL && current_mcb->next_node->status == 0 ){
        //Increase the size
        current_mcb->size += current_mcb->next_node->size + sizeof(mcb);
       
        //Update the next node pointer if applicable
        if(current_mcb->next_node->next_node != NULL){
            current_mcb->next_node->next_node->prev_node = current_mcb;
        }
        else{
            MCB_LIST.tail = current_mcb;
        }


        //Update its next pointer
        current_mcb->next_node = current_mcb->next_node->next_node;

    }

    /* Check if combinable with previous node*/
    if(current_mcb->prev_node != NULL && current_mcb->prev_node->status == 0){
        //Update Size
        current_mcb->size += current_mcb->prev_node->size + sizeof(mcb);

        //Update Stack location
        current_mcb->start_addr = current_mcb->prev_node->start_addr;

        //Update the next node pointer if applicable
        if(current_mcb->prev_node->prev_node != NULL){
            current_mcb->prev_node->prev_node->next_node = current_mcb;
        }
        else{
            MCB_LIST.head = current_mcb;
        }

        //Update current previous node pointer
        current_mcb->prev_node = current_mcb->prev_node->prev_node;
        
    }

    return 0;
}


void mcb_insert(mcb* new_mcb){

    // Check to see if passed mcb is null
    if(new_mcb == NULL){
        print_error("\nMCB cannot be passed to list \n");
        return;
    }

    // Check to see if list is empty
    if(MCB_LIST.head == NULL){
        //Insert into list at top
        MCB_LIST.head = new_mcb;
        MCB_LIST.tail = new_mcb;
        return;
    }

    //Find spot for it in list
    mcb* tempPtr = MCB_LIST.head;
    // mcb * combined_mcb;


    /*PLACE AT START*/
    if(new_mcb->start_addr < tempPtr->start_addr){
        //Check if they combine for free
        
        //Place before the first mcb in the list and don't combine
        new_mcb->next_node = tempPtr;

        //Check if previous is NULL
        if(tempPtr->prev_node == NULL){
            new_mcb->prev_node = NULL;
            MCB_LIST.head = new_mcb;
        }
        else{
            tempPtr->prev_node->next_node = new_mcb;
            new_mcb->prev_node = tempPtr->prev_node;
        }
        
        tempPtr->prev_node = new_mcb;

        tempPtr = NULL;
        
        return;

    }

    /*CHECK FUTURE MCBs*/
    while(tempPtr != NULL){
        /*CHECK IF AT END OF LIST*/
        if(tempPtr->next_node == NULL){
            //At end of list
            tempPtr->next_node = new_mcb;
            new_mcb->prev_node = tempPtr;
            
            MCB_LIST.tail = new_mcb;
            return;
        }

        /*CHECK IF NEW BLOCK FITS BETWEEN CURRENT AND NEXT*/
        if(new_mcb->start_addr < tempPtr->next_node->start_addr){
            new_mcb->next_node = tempPtr->next_node;
            tempPtr->next_node->prev_node = new_mcb;

            new_mcb->prev_node = tempPtr;
            tempPtr->next_node = new_mcb;

            tempPtr = NULL;
            return;
        }

        //Go to next mcb
        tempPtr=tempPtr->next_node;

    }
    //SOMETHING IS WRONG (If we get to this spot)!!!
}




void mcb_remove(mcb* current_mcb) {
    // Check if passed mcb is NULL
    if (current_mcb == NULL  || MCB_LIST.head == NULL) {
        print_error("MCB could not be removed from list\n");
        return; // nothing to remove
    }

    // Check if current mcb is the head
    if (current_mcb == MCB_LIST.head) {

        // the next node is the new head
        MCB_LIST.head = current_mcb->next_node;

        // if list is not empty after removing
        if (MCB_LIST.head != NULL) {
            MCB_LIST.head->prev_node = NULL;
        } 
        // if list is empty after removing
        else {
            MCB_LIST.tail = NULL; // list is now empty
        }
    } 
    
    // if MCB is tail
    else if (current_mcb == MCB_LIST.tail) { 

        // the previous node is the new tail
        MCB_LIST.tail = current_mcb->prev_node;

        // if list is not empty after removing
        if (MCB_LIST.tail != NULL) {
            MCB_LIST.tail->next_node = NULL;
        } 
        
        // if list is empty after removing
        else {
            MCB_LIST.head = NULL;
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
    mcb* current_mcb = MCB_LIST.head;
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

