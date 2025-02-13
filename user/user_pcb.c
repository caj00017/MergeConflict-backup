#include <mpx/pcb.h>
#include <sys_req.h>
#include <stdlib.h>

/**
 * Create PCB
 */
int create_PCB(char* name, int class, int priority){
    //Check name,class, and priority are correct or not used
    if(name == 0 || class == 0 || priority == 1){ //Placeholder so it doesn't yell at me for not using name
        return 1;
    }
    
    //Call PCB Setup
    pcb = pcb_setup(name, class, priority);

    //Insert into queue with PCB insert
    pcb_insert(pcb);

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
    
    //Remove from queue with pcb_remove
    pcb_remove(pcb);

    //free memory with pcb_free
    pcb_free(pcb);

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
    
    //Change to block state
    pcb = pcb_find(name);
    if (pcb == NULL) {
        return 1; // pcb not found
    }

    if (pcb->state == 0) {
        pcb->state == 3;
    }
    else if (pcb->state == 1) {
        pcb->state == 4;
    } else {
        return 1; // pcb cannot be blocked
    }

    //Put into appropriate queue
    pcb_insert(pcb);

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

    pcb = pcb_find(name);
    if (pcb == NULL) {
        return 1; // pcb not found
    }
    
    //Alter to ready
    if (pcb->state == 3) {
        pcb->state == 0;
    }
    else if (pcb->state == 4) {
        pcb->state == 1;
    } else {
        return 1; // pcb cannot be unblocked
    }

    //Insert into ready queue
    pcb_insert(pcb);

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
    
    //Alter to suspended state
    if (pcb->state == 1) {
        pcb->state = 0;
    }
    if (pcb->state == 4) {
        pcb->state = 3;
    }
    else {
        return 1; // pcb cannot be suspended
    }

    //Insert into appropriate queue
    pcb_insert(pcb);

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
    
    //Alter to not be suspended state
    if (pcb->state == 0) {
        pcb->state = 1;
    } else if (pcb->state == 3) {
        pcb->state = 4;
    } else {
        return 1; // pcb cannot be resumed
    }

    //Move to correct queue
    pcb_insert(pcb);

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
    pcb = pcb_find(name);
    if (pcb == NULL) {
        return 1; // pcb not found
    }

    // remove pcb for reinsertion?
    pcb_remove(pcb);

    //Change Priority
    pcb->priority = priority;

    // reinsert at proper location in queue
    pcb_insert(pcb);

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
    pcb = pcb_find(name);
    if (pcb == NULL) {
        return 1; // pcb not found
    }
    
    //Display Name, Class, State, Suspended Status, Priority
    sys_req(WRITE, COM1, name, sizeof(name)); // name is already char*
    sys_req(WRITE, COM1, itoa(pcb->class), sizeof(itoa(pcb->class))); // class is int; use itoa()
    sys_req(WRITE, COM1, itoa(pcb->state), sizeof(itoa(pcb->state))); // state is int; use itoa()
    sys_req(WRITE, COM1, itoa(pcb->priority), sizeof(itoa(pcb->priority))); // priority is int; use itoa()

    return 0;
}


 /**
 * Show Ready PCBs
 */
int show_ready_PCB(void){
    //For each process in Ready State, display the process

    return 0;
}


 /**
 * Show Blocked PCBs
 */
int show_blocked_PCB(void){
    //For each process in Blocked State, display the process

    return 0;
}


 /**
 * Show All PCBs
 */
int show_all_PCB(void){
    //Show each processes

    return 0;
}
