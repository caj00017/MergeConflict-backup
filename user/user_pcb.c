#include <mpx/pcb.h>
#include <mpx/user_pcb.h>
#include <sys_req.h>
#include <stdlib.h>
#include <stddef.h>

/**
 * Create PCB
 */
int create_PCB(char* name, int class, int priority){
    //Check name,class, and priority are correct or not used
    if(name == 0 || class == 0 || priority == 1){ //Placeholder so it doesn't yell at me for not using name
        return 1;
    }
    
    //Call PCB Setup
    pcb* PCB = pcb_setup(name, class, priority);

    //Check that PCB was created

    //Insert into queue with PCB insert
    pcb_insert(PCB);

    return 0;
}

/**
 * Delete PCB
 */
int delete_PCB(char* name){
    //Check name is correct or not used
    if(name == 0){
        return 1;
    }
    
    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }

    //Remove from queue with pcb_remove
    pcb_remove(PCB);

    //free memory with pcb_free
    pcb_free(PCB);

    return 0;
}



 /**
 * Block PCB
 */
int block_PCB(char* name){
    //Check name is correct or not used
    if(name == 0){
        return 1;
    }
    
    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }

    if (PCB->state == 0) {
        PCB->state = 3;
    }
    else if (PCB->state == 1) {
        PCB->state = 4;
    } else {
        return 1; // pcb cannot be blocked
    }

    //Put into appropriate queue
    pcb_insert(PCB);

    return 0;
}


 /**
 * Unblock PCB
 */
int unblock_PCB(char* name){
    //Check name is correct or not used
    if(name == 0){
        return 1;
    }

    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }
    
    //Alter to ready
    if (PCB->state == 3) {
        PCB->state = 0;
    }
    else if (PCB->state == 4) {
        PCB->state = 1;
    } else {
        return 1; // pcb cannot be unblocked
    }

    //Insert into ready queue
    pcb_insert(PCB);

    return 0;
}


 /**
 * Suspend PCB
 */
int suspend_PCB(char* name){
    //Check name is correct or not used
    if(name == 0){
        return 1;
    }
    
    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }
    
    //Alter to suspended state
    if (PCB->state == 1) {
        PCB->state = 0;
    }
    if (PCB->state == 4) {
        PCB->state = 3;
    }
    else {
        return 1; // pcb cannot be suspended
    }

    //Insert into appropriate queue
    pcb_insert(PCB);

    //???Must not be system process?

    return 0;
}


 /**
 * Resume PCB
 */
int resume_PCB(char* name){
    //Check name is correct or not used
    if(name == 0){
        return 1;
    }
    
    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }

    //Alter to not be suspended state
    if (PCB->state == 0) {
        PCB->state = 1;
    } else if (PCB->state == 3) {
        PCB->state = 4;
    } else {
        return 1; // pcb cannot be resumed
    }

    //Move to correct queue
    pcb_insert(PCB);

    return 0;
}


 /**
 * Set PCB Priority
 */
int set_PCB_priority(char* name, int priority){
    //Check name and priority are correct 
    if(name == 0 || priority == 1){
        return 1;
    }
    
    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }

    // remove pcb for reinsertion?
    pcb_remove(PCB);

    //Change Priority
    PCB->priority = priority;

    // reinsert at proper location in queue
    pcb_insert(PCB);

    return 0;
}



 /**
 * Show PCB
 */
int show_PCB(char* name){
    //Check name is correct 
    if(name == 0){
        return 1;
    }

    // get pcb
    pcb* PCB = pcb_find(name);
    if (PCB == NULL) {
        return 1; // pcb not found
    }
    
    //Display Name, Class, State, Suspended Status, Priority
    sys_req(WRITE, COM1, name, sizeof(name)); // name is already char*
    sys_req(WRITE, COM1, itoa(PCB->class,NULL,10), sizeof(itoa(PCB->class,NULL,10))); // class is int; use itoa()
    sys_req(WRITE, COM1, itoa(PCB->state,NULL,10), sizeof(itoa(PCB->state,NULL,10))); // state is int; use itoa()
    sys_req(WRITE, COM1, itoa(PCB->priority,NULL,10), sizeof(itoa(PCB->priority,NULL,10))); // priority is int; use itoa()

    return 0;
}


 /**
 * Show Ready PCBs
 */
int show_ready_PCB(void){
    //For each process in Ready State, display the process
    queue* ready = return_queue(0);
    pcb* current = ready->head;
    if (current == NULL) {
        sys_req(WRITE, COM1, "No processes in ready queue", sizeof("No processes in ready queue"));
    }
    while (current != NULL) {
        show_PCB(current->name);
        current = current->next_node;
    }

    return 0;
}


 /**
 * Show Blocked PCBs
 */
int show_blocked_PCB(void){
    //For each process in Blocked State, display the process
    queue* blocked = return_queue(1);
    pcb* current = blocked->head;
    if (current == NULL) {
        sys_req(WRITE, COM1, "No processes in blocked queue", sizeof("No processes in blocked queue"));
    }
    while (current != NULL) {
        show_PCB(current->name);
        current = current->next_node;
    }

    return 0;
}


 /**
 * Show All PCBs
 */
int show_all_PCB(void){
    //Show each processes

    // Ready
    sys_req(WRITE, COM1, "Ready Processes:", sizeof("Ready Processes:"));
    show_ready_PCB();

    // Blocked
    sys_req(WRITE, COM1, "Blocked Processes:", sizeof("Blocked Processes:"));
    show_blocked_PCB();

    return 0;
}
