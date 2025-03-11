#include <mpx/pcb.h>
#include <mpx/user_pcb.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>
#include <processes.h>

/**
 * Create PCB
 */
int create_PCB(char* name, int class, int priority){
    //Check name,class, and priority are correct or not used
    // if(name == 0 || class == 0 || priority == 1){ //Placeholder so it doesn't yell at me for not using name
    //     return 1;
    // }

    //Checks to see if PCb has already been created
    if (pcb_find(name) != NULL)
    {
        println();
        print_error("Error: PCB name already in use.");
        return 1;
    }

    //Call PCB Setup
    pcb* PCB = pcb_setup(name, class, priority, proc1);


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

    // ensure not a kernel process
    if (PCB->class == 1) {
        sys_req(WRITE, COM1, "\nError: Cannot alter a kernel process.", sizeof("\nError: Cannot alter a kernel process."));
        return 1;
    }

    //Remove from queue with pcb_remove
    pcb_remove(PCB);

    println();
    print("PCB Deleted: ");
    print(PCB->name);
    if (PCB->state == READY_SUS || PCB->state == READY_NOT_SUS) {
        show_ready_PCB();
    }
    else {
        show_blocked_PCB();
    }

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

    // ensure not a kernel process
    if (PCB->class == 1) {
        sys_req(WRITE, COM1, "\nError: Cannot alter a kernel process.", sizeof("\nError: Cannot alter a kernel process."));
        return 1;
    }


    // Remove from ready queue
    pcb_remove(PCB);// pcb_remove is currently not working

    if (PCB->state == 0) {
        PCB->state = 3;
    }
    else if (PCB->state == 1) {
        PCB->state = 4;
    } else {
        return 1; // pcb cannot be blocked
    }

    //Put into blocked queue
    pcb_insert(PCB);

    println();
    print("PCB Blocked: ");
    show_PCB(name);

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

    // ensure not a kernel process
    if (PCB->class == 1) {
        sys_req(WRITE, COM1, "\nError: Cannot alter a kernel process.", sizeof("\nError: Cannot alter a kernel process."));
        return 1;
    }
    
    // Remove from blocked queue
    pcb_remove(PCB); //pcb_remove is currently not working

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

    println();
    print("PCB Unblocked: ");
    show_PCB(name);

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

    // ensure not a kernel process
    if (PCB->class == 1) {
        sys_req(WRITE, COM1, "\nError: Cannot alter a kernel process.", sizeof("\nError: Cannot alter a kernel process."));
        return 1;
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

    //???Must not be system process?

    println();
    print("PCB Suspended: ");
    show_PCB(name);

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

    // ensure not a kernel process
    if (PCB->class == 1) {
        sys_req(WRITE, COM1, "\nError: Cannot alter a kernel process.", sizeof("\nError: Cannot alter a kernel process."));
        return 1;
    }

    //Alter to not be suspended state
    if (PCB->state == 0) {
        PCB->state = 1;
    } else if (PCB->state == 3) {
        PCB->state = 4;
    } else {
        return 1; // pcb cannot be resumed
    }

    println();
    print("PCB Resumed: ");
    show_PCB(name);

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

    // ensure not a kernel process
    if (PCB->class == 1) {
        sys_req(WRITE, COM1, "\nError: Cannot alter a kernel process.", sizeof("\nError: Cannot alter a kernel process."));
        return 1;
    }

    // remove pcb for reinsertion
    pcb_remove(PCB);

    // change Priority
    PCB->priority = priority;

    // reinsert at proper location in queue
    pcb_insert(PCB);

    println();
    print("PCB Updated: ");
    show_PCB(name);

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

    // non-null str for testing
    char str[100];

    char* class_str;
    char* state_str;
    char* priority_str = itoa(PCB->priority,str,10);

    switch(PCB->class) {
        case 0:
            class_str = "User";
            break;
        case 1:
            class_str = "Kernel";
            break;
        default:
            class_str = "Error";
            break;
    }

    switch (PCB->state) {
        case 0:
            state_str = "Ready, Suspended";
            break;
        case 1:
            state_str = "Ready, Not Suspended";
            break;
        case 2:
            state_str = "Running";
            break;
        case 3:
            state_str = "Blocked, Suspended";
            break;
        case 4:
            state_str = "Blocked, Not Suspended";
            break;
        default:
            state_str = "Error";
            break;
    }
    
    //Display Name, Class, State, Suspended Status, Priority
    print("\nName: ");
    print(PCB->name); // name is already char*
    println();

    print("Class: ");
    print(class_str);
    println();

    print("State: ");
    print(state_str); 
    println();

    print("Priority: ");
    print(priority_str);

    return 0;
}


 /**
 * Show Ready PCBs
 */
int show_ready_PCB(void){

    print("\n==========================");
    print("\nReady Processes:");
    print("\n==========================");

    //For each process in Ready State, display the process
    queue* ready = return_queue(0);
    pcb* current = ready->head;
    if (current == NULL) {
        println();
        print("No processes in ready queue");
    }
    while (current != NULL) {
        show_PCB(current->name);
        current = current->next_node;
        println();
    }

    return 0;
}


 /**
 * Show Blocked PCBs
 */
int show_blocked_PCB(void){

    print("\n==========================");
    print("\nBlocked Processes:");
    print("\n==========================");

    //For each process in Blocked State, display the process
    queue* blocked = return_queue(1);
    pcb* current = blocked->head;
    if (current == NULL) {
        println();
        print("No processes in blocked queue");
    }
    while (current != NULL) {
        show_PCB(current->name);
        current = current->next_node;
        println();
    }

    return 0;
}


 /**
 * Show All PCBs
 */
int show_all_PCB(void){
    //Show each processes

    // Ready
    show_ready_PCB();

    // Blocked
    show_blocked_PCB();

    return 0;
}



int Load_R3(void){
    //Might need error echecking to see if processes already exist
    if(  (pcb_find("Process 1") != NULL) || 
         (pcb_find("Process 2") != NULL) ||
         (pcb_find("Process 3") != NULL) || 
         (pcb_find("Process 4") != NULL) || 
         (pcb_find("Process 5") != NULL)    )
    {
        println();
        print_error("Processes 1-5 have already been created.");
        return 1;
    }

    //Manually make 5 processes with pcb_setup
    pcb* PCB1 = pcb_setup("Process 1", 0, 1, proc1);
    pcb* PCB2 = pcb_setup("Process 2", 0, 2, proc2);
    pcb* PCB3 = pcb_setup("Process 3", 0, 3, proc3);
    pcb* PCB4 = pcb_setup("Process 4", 0, 4, proc4);
    pcb* PCB5 = pcb_setup("Process 5", 0, 5, proc5);

    
    //Insert into Queues
    pcb_insert(PCB1);
    pcb_insert(PCB2);
    pcb_insert(PCB3);
    pcb_insert(PCB4);
    pcb_insert(PCB5);

    return 0;
}
