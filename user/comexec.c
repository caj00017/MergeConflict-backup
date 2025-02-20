// Include required header files
#include <mpx/comexec.h>
#include <string.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <sys_req.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <mpx/time.h>
#include <mpx/user_pcb.h>

int comexec(char buf[]) {
    
    if (strcmp(buf, "shutdown") == 0 || strcmp(buf, "sd") == 0) {
        int status = shutdown();
        return status;
    }
    else if (strcmp(buf, "version") == 0 || strcmp(buf, "v") == 0) {
        version();
        return 0;
    }
    else if (strcmp(buf, "get_date") == 0 || strcmp(buf, "gd") == 0) {
        get_date();
        return 0;
    }
    else if (strcmp(buf, "set_date") == 0) {
        sys_req(WRITE, COM1, "\nPlease enter the date to be set (set_date MM/DD/YY): ", sizeof("\nPlease enter the date to be set (set_date MM/DD/YY): "));
        return 0;
    }
    else if (contains(buf, "set_date") == 1) {
        set_date(buf);
        return 0;
    }
    else if (strcmp(buf, "get_time") == 0 || strcmp(buf, "gt") == 0) {
        get_time();
        return 0;
    }
    else if (strcmp(buf, "set_time") == 0) {
        sys_req(WRITE, COM1, "\nPlease enter the time to be set (set_time HH:MM): ", sizeof("\nPlease enter the time to be set (set_time HH:MM): "));
        return 0;
    }
    else if (contains(buf, "set_time") == 1) {
        set_time(buf);
        return 0;
    }
    else if (strcmp(buf, "help") == 0) {
        help();
        return 0;
    } 

    /*  PCB Functions  */

    else if (strcmp(buf, "process_create") == 0 || strcmp(buf, "pc") == 0)
    {
        //Write a newline to command line.
        sys_req(WRITE, COM1, "\n", sizeof("\n"));

        //Create variable for a one time only command per shutdown call
        int first = 1;

        char* name;
        int class;
        int priority;

        // Name entry loop
        while (1)
        {
            // reset reponse on each loop
            char name_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                sys_req(WRITE, COM1, "@ Please enter the name of the process to create (or write 'cancel' to cancel): ", sizeof("@ Please enter the name of the process to create (or write 'cancel' to cancel): "));
            }

            //Write formatting for entry and read from the command line
            sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
            sys_req(READ, COM1, name_response, sizeof(name_response));

            // write reponse for testing purposes
            sys_req(WRITE, COM1, "\nReponse entered: ", sizeof("\nReponse entered: "));
            sys_req(WRITE, COM1, name_response, sizeof(name_response));

            // check for invalid format
            if(isNumeric(name_response)){
                sys_req(WRITE, COM1, "\n", sizeof("\n"));
                sys_req(WRITE, COM1, "@ Please retype your response: ", sizeof("@ Please retype your response: "));
                continue;
            }
            else if (strcmp(name_response, "cancel") == 0){
                // cancel
                sys_req(WRITE, COM1, "\nProcess creation cancelled.", sizeof("\nProcess creation cancelled."));
                return 0;
            }
            else{
                // valid input
                name = name_response;
                break;
            }
        }

        // reset first
        first = 1;

        // Class entry loop
        while(1) {

            // reset reponse on each loop
            char class_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                sys_req(WRITE, COM1, "\n@ Please enter the class [0,1] of the process to create (or write 'cancel' to cancel): ", sizeof("@ Please enter the class [0,1] of the process to create (or write 'cancel' to cancel): "));
            }

            //Write formatting for entry and read from the command line
            sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
            sys_req(READ, COM1, class_response, sizeof(class_response));

            // write reponse for testing purposes
            sys_req(WRITE, COM1, "\nReponse entered: ", sizeof("\nReponse entered: "));
            sys_req(WRITE, COM1, class_response, sizeof(class_response));

            // check for valid format
            if(isNumeric(class_response) == 1 && atoi(class_response) >= 0 && atoi(class_response) <= 1){
                // valid input
                class = atoi(class_response);
                break;
            }
            else if (strcmp(class_response, "cancel") == 0){
                // cancel
                sys_req(WRITE, COM1, "\nProcess creation cancelled.", sizeof("\nProcess creation cancelled."));
                return 0;
            }
            else{
                // invalid input
                sys_req(WRITE, COM1, "\n", sizeof("\n"));
                sys_req(WRITE, COM1, "@ Please retype your response [0,1]: ", sizeof("@ Please retype your response: [0,1]"));
                continue;
            }
                
        }

        // reset first
        first = 1;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                sys_req(WRITE, COM1, "\n@ Please enter the priority [0-9] of the process to create (or write 'cancel' to cancel): ", sizeof("@ Please enter the priority [0-9] of the process to create (or write 'cancel' to cancel): "));
            }

            //Write formatting for entry and read from the command line
            sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
            sys_req(READ, COM1, priority_response, sizeof(priority_response));

            // write reponse for testing purposes
            sys_req(WRITE, COM1, "\nReponse entered: ", sizeof("\nReponse entered: "));
            sys_req(WRITE, COM1, priority_response, sizeof(priority_response));

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                sys_req(WRITE, COM1, "\nProcess creation cancelled.", sizeof("\nProcess creation cancelled."));
                return 0;
            }
            else{
                // invalid input
                sys_req(WRITE, COM1, "\n", sizeof("\n"));
                sys_req(WRITE, COM1, "@ Please retype your response [0-9]: ", sizeof("@ Please retype your response: [0-9]"));
                continue;
            }

        }

        int status = create_PCB(name, class, priority);
        if (status == 1) {
            sys_req(WRITE, COM1, "\nError creating PCB.", sizeof("\nError creating PCB."));
            return 0; // continue running comexec
        }

        sys_req(WRITE, COM1, "\nPCB created successfully.", sizeof("\nPCB created successfully."));
        show_PCB(name);
        return 0;
    }

    else if (strcmp(buf, "process_delete") == 0 || strcmp(buf, "pd") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the process to delete: ", sizeof("\nPlease enter the name of the process to delete: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else {
            sys_req(WRITE, COM1, "\nDeleting PCB: ", sizeof("\nDeleting PCB: "));
            sys_req(WRITE, COM1, name, strlen(name));
            delete_PCB(name);
            return 0;
        }
    }

    // CJ - Note: As of Feb. 19, pcb_remove is not working, so pcb_block is unable to remove blocked PCBs from the ready queue
    else if (strcmp(buf, "process_block") == 0 || strcmp(buf, "pb") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the process to block: ", sizeof("\nPlease enter the name of the process to block: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else if (PCB->state != READY_SUS && PCB->state != READY_NOT_SUS) {
            sys_req(WRITE, COM1, "\nProcess is not in a ready state: ", sizeof("\nProcess is not in a ready state: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else {
            sys_req(WRITE, COM1, "\nBlocking PCB: ", sizeof("\nBlocking PCB: "));
            sys_req(WRITE, COM1, name, strlen(name));
            block_PCB(name);
            return 0;
        }
    }

    // CJ - Note: As of Feb. 19, pcb_remove is not working, so pcb_unblock is unable to remove ready PCBs from the blocked queue
    // Side note: Executing 'psa' or any similar command after unblocking results in an infinite loop. No idea why.
    else if (strcmp(buf, "process_unblock") == 0 || strcmp(buf, "pub") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the process to unblock: ", sizeof("\nPlease enter the name of the process to unblock: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else if (PCB->state != BLOCKED_SUS && PCB->state != BLOCKED_NOT_SUS) {
            sys_req(WRITE, COM1, "\nProcess is not in a blocked state: ", sizeof("\nProcess is not in a blocked state: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else {
            sys_req(WRITE, COM1, "\nUnblocking PCB: ", sizeof("\nUnblocking PCB: "));
            sys_req(WRITE, COM1, name, strlen(name));
            unblock_PCB(name);
            return 0;
        }
    }

    else if (strcmp(buf, "process_suspend") == 0 || strcmp(buf, "psus") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the process to suspend: ", sizeof("\nPlease enter the name of the process to suspend: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else if (PCB->state != READY_NOT_SUS && PCB->state != BLOCKED_NOT_SUS) {
            sys_req(WRITE, COM1, "\nProcess is already suspended: ", sizeof("\nProcess is already suspended: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else {
            sys_req(WRITE, COM1, "\nSuspending PCB: ", sizeof("\nSuspending PCB: "));
            sys_req(WRITE, COM1, name, strlen(name));
            suspend_PCB(name);
            return 0;
        }
    }

    else if (strcmp(buf, "process_resume") == 0 || strcmp(buf, "pres") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the suspended process to resume: ", sizeof("\nPlease enter the name of the suspended process to resume: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else if (PCB->state != READY_SUS && PCB->state != BLOCKED_SUS) {
            sys_req(WRITE, COM1, "\nProcess cannot be resumed: ", sizeof("\nProcess cannot be resumed: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else {
            sys_req(WRITE, COM1, "\nResuming PCB: ", sizeof("\nResuming PCB: "));
            sys_req(WRITE, COM1, name, strlen(name));
            resume_PCB(name);
            return 0;
        }
    }

    else if (strcmp(buf, "process_priority") == 0 || strcmp(buf, "pp") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the process to update: ", sizeof("\nPlease enter the name of the process to update: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }

        int first = 1;
        int priority;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                sys_req(WRITE, COM1, "\n@ Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ", sizeof("@ Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): "));
            }

            //Write formatting for entry and read from the command line
            sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
            sys_req(READ, COM1, priority_response, sizeof(priority_response));

            // write reponse for testing purposes
            sys_req(WRITE, COM1, "\nReponse entered: ", sizeof("\nReponse entered: "));
            sys_req(WRITE, COM1, priority_response, sizeof(priority_response));

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                sys_req(WRITE, COM1, "\nPriority reassignment cancelled.", sizeof("\nPriority reassignment cancelled."));
                return 0;
            }
            else{
                // invalid input
                sys_req(WRITE, COM1, "\n", sizeof("\n"));
                sys_req(WRITE, COM1, "@ Please retype your response [0-9]: ", sizeof("@ Please retype your response: [0-9]"));
                continue;
            }

        }

        char str[100];

        sys_req(WRITE, COM1, "\nSetting priority for PCB: ", sizeof("\nSetting priority for PCB: "));
        sys_req(WRITE, COM1, name, strlen(name));
        sys_req(WRITE, COM1, " to ", sizeof(" to "));
        sys_req(WRITE, COM1, itoa(priority, str, 10), sizeof(itoa(priority, str, 10)));

        set_PCB_priority(name, priority);

        return 0;
    }

    else if (strcmp(buf, "process_show") == 0 || strcmp(buf, "ps") == 0)
    {
        char* name;
        char response[100] = { 0 };
        sys_req(WRITE, COM1, "\nPlease enter the name of the process to show: ", sizeof("\nPlease enter the name of the process to show: "));

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        name = response;

        sys_req(WRITE, COM1, "\nLocating PCB: ", sizeof("\nLocating PCB: "));
        sys_req(WRITE, COM1, name, strlen(name));
        sys_req(WRITE, COM1, "...", sizeof("..."));
        
        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            sys_req(WRITE, COM1, "\nProcess not found: ", sizeof("\nProcess not found: "));
            sys_req(WRITE, COM1, name, strlen(name));
            return 0; // continue running comexec
        }
        else {
            show_PCB(name);
            return 0;
        }
    }

    else if (strcmp(buf, "process_show_ready") == 0 || strcmp(buf, "psr") == 0)
    {
        return show_ready_PCB();
    }

    else if (strcmp(buf, "process_show_blocked") == 0 || strcmp(buf, "psb") == 0)
    {
        return show_blocked_PCB();
    }

    else if (strcmp(buf, "process_show_all") == 0 || strcmp(buf, "psa") == 0)
    {
        return show_all_PCB();
    }

    else if (strcmp(buf, "test_color") == 0)
    {
        println();
        print_error("THis is an error");
        return 0;
    }

    //Bonus Commands
    else if (strcmp(buf, "clear") == 0) {
        clear_screen();
        return 5;
    }

    else {
        println();
        print_error("Error: Invalid command.");
        return 1;
    }
}

int shutdown(void) {
    //Write a newline to command line.
    sys_req(WRITE, COM1, "\n", sizeof("\n"));

    //Create variable for a one time only command per shutdown call
    int first = 1;

    //Loop as long as the person keeps mispelling / failed entry
    while (1)
    {
        //Character response for double check
        char response[100] = { 0 };

        //For first time, write instructions
        if(first== 1){
            first++;
            sys_req(WRITE, COM1, "@ Are you sure you want to shutdown? (y or n)", sizeof("@ Are you sure you want to shutdown? (y or n)"));
        }

        //Write formatting for entry and read from the command line
        sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
        sys_req(READ, COM1, response, sizeof(response));

        //Check to see if the person want to shutdown (y) or continue running program(n)
        if(strcmp(response, "y") == 0 || strcmp(response, "yes") == 0 ){
            return -1; //exit and end the program
        }
        else if(strcmp(response, "n") == 0 || strcmp(response, "no") == 0 ){
            sys_req(WRITE, COM1, "\n@ Returning to Usual Operations...", sizeof("\n@ Returning to Usual Operations..."));
            return 1; //exit and continue running program
        }
        else{
            //If they don't response with the proper entry, give repeated instructions and loop again.
            sys_req(WRITE, COM1, "\n", sizeof("\n"));
            sys_req(WRITE, COM1, "@ Please retype your response: (y or n)", sizeof("@ Please retype your response: (y or n)"));
            continue;
        }
    
        return 0;
    }
}

int help(void) {
    // Print list of commands

    // R1 commands
    sys_req(WRITE, COM1, "\n@ help\t\tPrints a complete list of each available command.", sizeof("@ help\tPrints a complete list of each available command."));
    sys_req(WRITE, COM1, "\n@ version\tPrints the current version of the program.", sizeof("@ version\tPrints the current version of the program."));
    sys_req(WRITE, COM1, "\n@ get_date\tPrints the current date set by the user.", sizeof("@ get_date\tPrints the current date set by the user."));
    sys_req(WRITE, COM1, "\n@ set_date\tMM/DD/YY\tSets the current date.", sizeof("@ set_date\tMM/DD/YY\tSets the current date."));
    sys_req(WRITE, COM1, "\n@ get_time\tPrints the current time set by the user.", sizeof("@ get_time\tPrints the current time set by the user."));
    sys_req(WRITE, COM1, "\n@ set_time\tHH:MM:SS\tSets the current time.", sizeof("@ set_time\tHH:MM:SS\tSets the current time."));
    sys_req(WRITE, COM1, "\n@ shutdown\tExits the program.", sizeof("\n@ shutdown\tExits the program."));

    // R2 commands
    sys_req(WRITE, COM1, "\n", sizeof("\n"));
    sys_req(WRITE, COM1, "\n@ process_create\tpc\tCreates a new process.", sizeof("\n@ process_create\tpc\tCreates a new process."));
    sys_req(WRITE, COM1, "\n@ process_delete\tpd\tDeletes a process.", sizeof("\n@ process_delete\tpd\tDeletes a process."));
    sys_req(WRITE, COM1, "\n@ process_block\t\tpb\tBlocks a process.", sizeof("\n@ process_block\t\tpb\tBlocks a process."));
    sys_req(WRITE, COM1, "\n@ process_unblock\tpub\tUnblocks a process.", sizeof("\n@ process_unblock\tpub\tUnblocks a process."));
    sys_req(WRITE, COM1, "\n@ process_suspend\tpsus\tSuspends a process.", sizeof("\n@ process_suspend\tpsus\tSuspends a process."));
    sys_req(WRITE, COM1, "\n@ process_resume\tpres\tResumes a process.", sizeof("\n@ process_resume\tpres\tResumes a process."));
    sys_req(WRITE, COM1, "\n@ process_priority\tpp\tChanges the priority of a process.", sizeof("\n@ process_priority\tpp\tChanges the priority of a process."));
    sys_req(WRITE, COM1, "\n@ process_show\t\tps\tShows information about a process.", sizeof("\n@ process_show\t\tps\tShows information about a process."));
    sys_req(WRITE, COM1, "\n@ process_show_ready\tpsr\tShows all processes in the ready state.", sizeof("\n@ process_show_ready\tpsr\tShows all processes in the ready state."));
    sys_req(WRITE, COM1, "\n@ process_show_blocked\tpsb\tShows all processes in the blocked state.", sizeof("\n@ process_show_blocked\tpsb\tShows all processes in the blocked state."));
    sys_req(WRITE, COM1, "\n@ process_show_all\tpsa\tShows all processes.", sizeof("\n@ process_show_all\tpsa\tShows all processes."));
    return 0;
}

int version(void) {
    char msg[100] = "\nVersion 1.0\nCompilation Date: 2/7/2025";
    sys_req(WRITE,COM1, msg, sizeof(msg));
    return 0;
}

int clear_screen(void) {
    sys_req(WRITE, COM1, "\033[2J", sizeof("\033[H"));
    sys_req(WRITE, COM1, "\033[H", sizeof("\033[H"));
    return 0;
}
