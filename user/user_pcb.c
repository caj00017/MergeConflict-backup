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
    pcb* PCB = pcb_setup(name, class, priority, 1, proc1, NULL);


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
        print_error("\nError: Cannot alter a kernel process.");
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
    println();

    // Check for Process 1
    if( pcb_find("Process 1") == NULL){
        pcb* PCB1 = pcb_setup("Process 1", 1, 1, 1, proc1, NULL); //Name, Class, Priority, State, Function
        print("Finished Loading: Process 1\n");
        pcb_insert(PCB1);
    }
    else{
        print_error("Processes 1 has already been created.\n");
    }

    // Check for Process 2
    if( pcb_find("Process 2") == NULL){
        pcb* PCB2 = pcb_setup("Process 2", 1, 2, 1, proc2, NULL); //Name, Class, Priority, State, Function
        print("Finished Loading: Process 2\n");
        pcb_insert(PCB2);
    }
    else{
        print_error("Processes 2 has already been created.\n");
    }

    // Check for Process 3
    if( pcb_find("Process 3") == NULL){
        pcb* PCB3 = pcb_setup("Process 3", 1, 3, 1, proc3, NULL); //Name, Class, Priority, State, Function
        print("Finished Loading: Process 3\n");
        pcb_insert(PCB3);
    }
    else{
        print_error("Processes 3 has already been created.\n");
    }

    // Check for Process 4
    if( pcb_find("Process 4") == NULL){
        pcb* PCB4 = pcb_setup("Process 4", 1, 4, 1, proc4, NULL); //Name, Class, Priority, State, Function
        print("Finished Loading: Process 4\n");
        pcb_insert(PCB4);
    }
    else{
        print_error("Processes 4 has already been created.\n");
    }

    // Check for Process 5
    if( pcb_find("Process 5") == NULL){
        pcb* PCB5 = pcb_setup("Process 5", 1, 5, 1, proc5, NULL); //Name, Class, Priority, State, Function
        print("Finished Loading: Process 5");
        pcb_insert(PCB5);
    }
    else{
        print_error("Processes 5 has already been created.\n");
    }

    return 0;
}


int Load_R3_Sus(int pcb_num, int priority){

    void (*funct)(void) = NULL;
    char * pcb_name = NULL;

    //Check the users input for process and priority
    if(pcb_num > 5 || pcb_num <1){
        print_error("Process Number can not be used.");
        return 1;
    }
    else if(pcb_num == 1){
        pcb_name = "Process 1";
        funct = proc1;
    }
    else if(pcb_num == 2){
        pcb_name = "Process 2";
        funct= proc2;
    }
    else if(pcb_num == 3){
        pcb_name = "Process 3";
        funct = proc3;
    }
    else if(pcb_num == 4){
        pcb_name = "Process 4";
        funct = proc4;
    }
    else if(pcb_num == 5){
        pcb_name = "Process 5";
        funct = proc5;
    }

    
    //Might need error echecking to see if processes already exist
    if(pcb_find(pcb_name) != NULL)
    {
        println();
        print_error("This process has already been created.");
        return 1;
    }

    //Manually make process with pcb_setup
    pcb* PCB = pcb_setup(pcb_name, 0, priority, 0, funct, NULL);

    //Print status
    print("\nFinished Loading as Suspended: ");
    print(pcb_name);
   

    //Insert into Queues
    pcb_insert(PCB);

    show_PCB(pcb_name);

    return 0;
}

