
#include <mpx/pcb.h>

/**
 * Create PCB
 */
int create_PCB(char* name, int class, int priority){
    //Check name,class, and priority are correct or not used
    if(name == 0 || class == 0 || priority == 1){ //Placeholder so it doesn't yell at me for not using name
        return 1;
    }
    
    //Call PCB Setup

    //Insert into queue with PCB insert

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

    //free memory with pcb_free

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

    //Put into appropriate queue

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
    
    //Alter to ready

    //Insert into ready queue

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

    //Insert into appropriate queue

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

    //Move to correct queue

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
    
    //Change Priority

    //Move to proper spot in queue

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
    
    //Display Name, Class, State, Suspended Status, Priority

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
