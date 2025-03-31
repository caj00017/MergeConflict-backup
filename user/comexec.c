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
#include <mpx/sys_call.h>
#include <mpx/alarm.h>
#include <mcb.h>

char* color = "blue"; // color to be set by the user, blue by default

int comexec(void) {

    // Add the @ before each command
    print_color("@ ", color);

    // initialize buffer and read from user
    char buf[100] = { 0 };
    sys_req(READ, COM1, buf, sizeof(buf));
    trim(buf);
    
    // --------------------------------------------------------------------- // 
    // --------------------------- R1 COMMAND LOGIC ------------------------ //
    // --------------------------------------------------------------------- //

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
        print("\nPlease enter the date to be set (set_date MM/DD/YY): ");
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
        print("\nPlease enter the time to be set (set_time HH:MM:SS) ");
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
    else if (contains(buf, "color") == 1) {

        println();
        int first = 1;

        // color entry loop
        while (1)
        {
            // reset reponse on each loop
            char color_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                print_color("@ ", color);
                print("Color options: ");
                print_color("green, ", "green");
                print_color("yellow, ", "yellow");
                print_color("blue, ", "blue");
                print_color("magenta, ", "magenta");
                print_color("cyan\n", "cyan");

                print("Please enter the color to change to (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, color_response, sizeof(color_response));
            trim(color_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(color_response);

            // ensure valid input
            if(strcmp(color_response, "green") == 0 || strcmp(color_response, "yellow") == 0 || strcmp(color_response, "blue") == 0 || strcmp(color_response, "magenta") == 0 || strcmp(color_response, "cyan") == 0){
                color = color_response;
                print("\nColor changed to: ");
                print_color(color, color);
                break;
            }
            else if (strcmp(color_response, "cancel") == 0){
                // cancel
                println();
                print("Process creation cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response: ");
                continue;
            }
        }

        first = 1;
        return 0;
    }

    // --------------------------------------------------------------------- // 
    // --------------------------- R2 COMMAND LOGIC ------------------------ //
    // --------------------------------------------------------------------- //

    else if (strcmp(buf, "process_delete") == 0 || strcmp(buf, "pd") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the process to delete: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            println();
            print("Process not found: ");
            print(name);
            return 0; // continue running comexec
        }
        else {
            println();
            print("Deleting PCB: ");
            print(name);
            delete_PCB(name);
            return 0;
        }
    }
    else if (strcmp(buf, "process_block") == 0 || strcmp(buf, "pb") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the process to block: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            println();
            print("Process not found: ");
            print(name);
            return 0; // continue running comexec
        }
        else if (PCB->state != READY_SUS && PCB->state != READY_NOT_SUS) {
            println();
            print("Process is not in a ready state: ");
            print(name);
            return 0; // continue running comexec
        }
        else {
            println();
            print("Blocking PCB: ");
            print(name);
            block_PCB(name);
            return 0;
        }
    }
    else if (strcmp(buf, "process_unblock") == 0 || strcmp(buf, "pub") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the process to unblock: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            println();
            print("Process not found: ");
            print(name);
            return 0; // continue running comexec
        }
        else if (PCB->state != BLOCKED_SUS && PCB->state != BLOCKED_NOT_SUS) {
            println();
            print("Process is not in a blocked state: ");
            print(name);
            return 0; // continue running comexec
        }
        else {
            println();
            print("Unblocking PCB: ");
            print(name);
            unblock_PCB(name);
            return 0;
        }
    }
    else if (strcmp(buf, "process_suspend") == 0 || strcmp(buf, "psus") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the process to suspend: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        pcb* PCB = pcb_find(name);
        println();

        if (PCB == NULL) {
            print("Process not found: ");
            print(name);
            return 0; // continue running comexec
        }
        else if (PCB->state != READY_NOT_SUS && PCB->state != BLOCKED_NOT_SUS) {
            print("Process is already suspended: ");
            print(name);
            return 0; // continue running comexec
        }
        else {
            print("Suspending PCB: ");
            print(name);
            suspend_PCB(name);
            return 0;
        }
    }
    else if (strcmp(buf, "process_resume") == 0 || strcmp(buf, "pres") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the suspended process to resume: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        pcb* PCB = pcb_find(name);
        println();
        if (PCB == NULL) {
            print("Process not found: ");
            print(name);
            return 0; // continue running comexec
        }
        else if (PCB->state != READY_SUS && PCB->state != BLOCKED_SUS) {
            print("Process cannot be resumed: ");
            print(name);
            return 0; // continue running comexec
        }
        else {
            print("Resuming PCB: ");
            print(name);
            resume_PCB(name);
            return 0;
        }
    }
    else if (strcmp(buf, "process_priority") == 0 || strcmp(buf, "pp") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the process to update: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        pcb* PCB = pcb_find(name);
        println();

        if (PCB == NULL) {
            print("Process not found: ");
            print(name);
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
                println();
                print_color("@ ", color);
                print("Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, priority_response, sizeof(priority_response));
            trim(priority_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(priority_response);

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                println();
                print("Priority reassignment cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response [0-9]: ");
                continue;
            }

        }

        char str[100];

        println();
        print("Setting priority for PCB: ");
        print(name);
        print(" to ");
        print(itoa(priority, str, 10));

        set_PCB_priority(name, priority);

        return 0;
    }
    else if (strcmp(buf, "process_show") == 0 || strcmp(buf, "ps") == 0)
    {
        char* name;
        char response[100] = { 0 };
        println();
        print("Please enter the name of the process to show: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        name = response;

        println();
        print("Locating PCB: ");
        print(name);
        print("...");
        println();

        pcb* PCB = pcb_find(name);
        if (PCB == NULL) {
            print("Process not found: ");
            print(name);
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

    // --------------------------------------------------------------------- // 
    // ------------------------- R3 COMMAND LOGIC -------------------------- //
    // --------------------------------------------------------------------- // 

    else if (strcmp(buf, "load_r3") == 0 || strcmp(buf, "lr3") == 0)
    {
        return Load_R3();
    }
    else if (strcmp(buf, "load_p1") == 0 || strcmp(buf, "lp1") == 0)
    {
        int first = 1;
        int priority;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                println();
                print_color("@ ", color);
                print("Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, priority_response, sizeof(priority_response));
            trim(priority_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(priority_response);

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                println();
                print("Test Process Creation Cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response [0-9]: ");
                continue;
            }

        }

        return Load_R3_Sus(1, priority);
    }
    else if (strcmp(buf, "load_p2") == 0 || strcmp(buf, "lp2") == 0)
    {
        int first = 1;
        int priority;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                println();
                print_color("@ ", color);
                print("Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, priority_response, sizeof(priority_response));
            trim(priority_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(priority_response);

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                println();
                print("Test Process Creation Cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response [0-9]: ");
                continue;
            }

        }

        return Load_R3_Sus(2, priority);
    }
    else if (strcmp(buf, "load_p3") == 0 || strcmp(buf, "lp3") == 0)
    {
        int first = 1;
        int priority;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                println();
                print_color("@ ", color);
                print("Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, priority_response, sizeof(priority_response));
            trim(priority_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(priority_response);

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                println();
                print("Test Process Creation Cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response [0-9]: ");
                continue;
            }

        }

        return Load_R3_Sus(3, priority);
    }
    else if (strcmp(buf, "load_p4") == 0 || strcmp(buf, "lp4") == 0)
    {
        int first = 1;
        int priority;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                println();
                print_color("@ ", color);
                print("Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, priority_response, sizeof(priority_response));
            trim(priority_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(priority_response);

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                println();
                print("Test Process Creation Cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response [0-9]: ");
                continue;
            }

        }

        return Load_R3_Sus(4, priority);
    }
    else if (strcmp(buf, "load_p5") == 0 || strcmp(buf, "lp5") == 0)
    {
        int first = 1;
        int priority;

        // Priority entry loop
        while(1) {

            // reset reponse on each loop
            char priority_response[100] = { 0 };

            //For first time, write instructions
            if(first == 1){
                first = 0;
                println();
                print_color("@ ", color);
                print("Please enter the new priority [0-9] of the process (or write 'cancel' to cancel): ");
            }

            //Write formatting for entry and read from the command line
            println();
            print_color("@ ", color);
            sys_req(READ, COM1, priority_response, sizeof(priority_response));
            trim(priority_response);

            // write reponse for testing purposes
            println();
            print("Reponse entered: ");
            print(priority_response);

            // check for valid format
            if(isNumeric(priority_response) == 1 && atoi(priority_response) >= 0 && atoi(priority_response) <= 9){
                // valid input
                priority = atoi(priority_response);
                break;
            }
            else if (strcmp(priority_response, "cancel") == 0){
                // cancel
                println();
                print("Test Process Creation Cancelled.");
                return 0;
            }
            else{
                // invalid input
                println();
                print_color("@ ", color);
                print("Please retype your response [0-9]: ");
                continue;
            }

        }

        return Load_R3_Sus(5, priority);
    }
    
    // --------------------------------------------------------------------- // 
    // ------------------------- R4 COMMAND LOGIC -------------------------- //
    // --------------------------------------------------------------------- // 
    else if (strcmp(buf, "alarm") == 0 || strcmp(buf, "a") == 0) {
        char message[100] = { 0 };
        char time[9] = { 0 };

        println();
        print("Please enter the message for the alarm (100 chars. max): ");
        
        // Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, message, sizeof(message));
        message[99] = '\0'; // Ensure null termination
        trim(message);

        sys_req(WRITE, COM1, "\nPlease enter the time for the alarm (HH:MM:SS): ", sizeof("\nPlease enter the time for the alarm (HH:MM:SS): "));
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, time, sizeof(time));
        time[8] = '\0'; // Ensure null termination
        trim(time);

        // check for invalid format (invalid time check pending)
        char washed_time[100];
        strcpy(washed_time, time); // wash the time string to remove any leading/trailing spaces
        if (charCount(time, ':') != 2 || strlen(time) != 8 || isNumeric(strtok(time, ":")) == 0) {
            print("Invalid time format. (HH:MM:SS)");
            return 0;
        }
        else {
            char hr_chars[3] = {time[0], time[1], '\0'}; // first 2 chars = hour value
            char min_chars[3] = {time[3], time[4], '\0'}; // next 2 chars (skipping ':') = minute value
            char sec_chars[3] = {time[6], time[7], '\0'}; // last 2 chars = second value

            // check for invalid times / characters 
            if (strcmp(hr_chars, "23") > 0 || strcmp(hr_chars, "0") < 0   
            || strcmp(min_chars, "59") > 0 || strcmp(min_chars, "0") < 0  
            || strcmp(sec_chars, "59") > 0 || strcmp(sec_chars, "0") < 0) 
            {
                sys_req(WRITE, COM1, "\nInvalid time. ([00-23]:[00-59]:[00-59])", sizeof("\nInvalid time. ([00-23]:[00-59]:[00-59])"));
                return 0;
            }
        }

        println();
        print("Creating alarm...");
        println();

        print("Message: ");
        print(message);
        println();
        print("Time: ");
        print(washed_time);
        println();

        create_alarm(message, washed_time);
        return 0;
    }

    // --------------------------------------------------------------------- // 
    // ---------------------- R5 TEST COMMAND LOGIC ------------------------ //
    // --------------------------------------------------------------------- // 

    else if (strcmp(buf, "create_mcb") == 0 || strcmp(buf, "cm") == 0) {
        
        // prompt for address
        unsigned int start_addr;
        char addr_response[100] = { 0 };
        println();
        print("Please enter the address of the MCB to create: 0x");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, addr_response, sizeof(addr_response));
        trim(addr_response);

        start_addr = (unsigned int)atoi(addr_response);

        // prompt for size
        int size;
        char size_response[100] = { 0 };
        println();
        print("Please enter the size of the MCB to create: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, size_response, sizeof(size_response));
        trim(size_response);

        size = atoi(size_response);

        println();
        print("Creating MCB at: 0x");
        print(itoa(start_addr, addr_response, 10));
        print("...");

        mcb* new_mcb = mcb_setup(start_addr, size);
        list* free_list = return_list(0);
        mcb_insert(free_list, new_mcb);

        print("\nCreated new ");
        show_mcb(new_mcb);
        return 0;
    }
    else if (strcmp(buf, "delete_mcb") == 0 || strcmp(buf, "dm") == 0) {
        unsigned int addr;
        char response[100] = { 0 };
        println();
        print("Please enter the address of the MCB to delete: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        addr = (unsigned int)atoi(response);

        println();
        print("Deleting MCB at 0x");
        print(itoa(addr, response, 10));
        print("...\n");

        mcb* mcb = mcb_find(addr);
        if (mcb == NULL) {
            println();
            print("MCB not found at 0x");
            print(itoa(addr, response, 10));
            return 0; // continue running comexec
        }
        else {

            // this is bad practice, but who really cares anyway
            list* free_list = return_list(0);
            list* allocated_list = return_list(1);
            mcb_remove(free_list, mcb);
            mcb_remove(allocated_list, mcb);

            println();
            print("Deleted MCB at 0x");
            print(itoa(addr, response, 10));
        }
        return 0;
    }
    else if (strcmp(buf, "show_mcb") == 0 || strcmp(buf, "sm") == 0) {
        unsigned int addr;
        char response[100] = { 0 };
        println();
        print("Please enter the address of the MCB to show: ");

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        addr = (unsigned int)atoi(response);

        println();
        print("Locating MCB: ");
        print(itoa(addr, response, 10));
        print("...");
        println();

        mcb* mcb = mcb_find(addr);
        if (mcb == NULL) {
            print("MCB not found at 0x");
            print(itoa(addr, response, 10));
            return 0; // continue running comexec
        }

        println();
        show_mcb(mcb);

        return 0;
    }
    else if (strcmp(buf, "show_mcb_free") == 0 || strcmp(buf, "smf") == 0) {
        println();
        print("Showing free MCBs...");
        show_free_mem();
        return 0;
    }
    else if (strcmp(buf, "show_mcb_alloc") == 0 || strcmp(buf, "sma") == 0) {
        println();
        print("Showing allocated MCBs...");
        show_alloc_mem();
        return 0;
    }
    else if (strcmp(buf, "show_mcb_all") == 0 || strcmp(buf, "small") == 0) {
        println();
        print("Showing all MCBs...");
        show_alloc_mem();
        show_free_mem();
        return 0;
    }

    // --------------------------------------------------------------------- // 
    // ---------------------- BONUS + TEST COMMAND LOGIC ------------------- //
    // --------------------------------------------------------------------- // 

    else if (strcmp(buf, "test_trim") == 0)
    {
        char words[100] = "    shutit    ";
        println();
        print(words);
        trim(words);
        println();
        print(words);
        print("hello");
        return 0;
    }

    //Bonus Commands
    else if (strcmp(buf, "clear") == 0) {
        clear_screen();
        return 5;
    }

    else {
        println();
        print_error("Error: Invalid command. Please Re-type.");
        return 1;
    }
}

int shutdown(void) {
    //Write a newline to command line.
    println();

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
            print_color("@ ", color);
            print("Are you sure you want to shutdown? (y or n)");
        }

        //Write formatting for entry and read from the command line
        println();
        print_color("@ ", color);
        sys_req(READ, COM1, response, sizeof(response));
        trim(response);

        //Check to see if the person want to shutdown (y) or continue running program(n)
        if(strcmp(response, "y") == 0 || strcmp(response, "yes") == 0 ){
        //delete the ready queue and the blocked queue
        //clear the ready queue
        pcb* ready = return_queue(0)->tail;
        while(ready != NULL) {
            pcb* temp = ready->prev_node;
            pcb_remove(ready);
            pcb_free(ready);
            ready = temp;
        }
        //clear the blocked queue
        pcb* blocked = return_queue(1)->tail;
        while(blocked != NULL) {
            pcb* temp = blocked->prev_node;
            pcb_remove(blocked);
            pcb_free(blocked);
            blocked = temp;
        }
            //then exit
            sys_req(EXIT);
            return -1; //exit and end the program
        }
        else if(strcmp(response, "n") == 0 || strcmp(response, "no") == 0 ){
            println();
            print_color("@ ", color);
            print("Returning to Usual Operations...");
            return 1; //exit and continue running program
        }
        else{
            //If they don't response with the proper entry, give repeated instructions and loop again.
            println();
            print_color("@ ", color);
            print("Please retype your response: (y or n)");
            continue;
        }
    
        return 0;
    }
}

int help(void) {
    // Print list of commands

    // R1 commands
    println();
    println();
    print_color("===================================================================================================", color); //100 equal signs
    println();
    println();
    print_color("@ ", color);
    print("help\t\t\tPrints a complete list of each available command.");
    println(); 
    print_color("@ ", color);
    print("version\tv\tPrints the current version of the program.");
    println(); 
    print_color("@ ", color);
    print("get_date\tgd\tPrints the current date set by the user.");
    println(); 
    print_color("@ ", color);
    print("set_date\t\tMM/DD/YY\tSets the current date.");
    println(); 
    print_color("@ ", color);
    print("get_time\tgt\tPrints the current time set by the user.");
    println(); 
    print_color("@ ", color);
    print("set_time\t\tHH:MM:SS\tSets the current time.");
    println();
    print_color("@ ", color);
    print("alarm \ta\tCreates an alarm to go off at the specified time");
    println();
    print_color("@ ", color);
    print("color\t\t\tChanges the text color.");    
    println();
    print_color("@ ", color);
    print("clear\t\t\tClears the console.");
    println();
    print_color("@ ", color);
    print("shutdown\tsd\tExits the program.");


    // R2 commands
    println();
    println();
    print_color("===================================================================================================", color);
    println();
    println(); 
    print_color("@ ", color);
    print("process_delete\tpd\tDeletes a process.");
    println(); 
    print_color("@ ", color);
    print("process_block\t\tpb\tBlocks a process.");
    println(); 
    print_color("@ ", color);
    print("process_unblock\tpub\tUnblocks a process.");
    println(); 
    print_color("@ ", color);
    print("process_suspend\tpsus\tSuspends a process.");
    println(); 
    print_color("@ ", color);
    print("process_resume\tpres\tResumes a process.");
    println(); 
    print_color("@ ", color);
    print("process_priority\tpp\tChanges the priority of a process.");
    println(); 
    print_color("@ ", color);
    print("process_show\t\tps\tShows information about a process.");
    println(); 
    print_color("@ ", color);
    print("process_show_ready\tpsr\tShows all processes in the ready state.");
    println(); 
    print_color("@ ", color);
    print("process_show_blocked\tpsb\tShows all processes in the blocked state.");
    println(); 
    print_color("@ ", color);
    print("process_show_all\tpsa\tShows all processes.");

    //Load R3 Commands
    println();
    println();
    print_color("===================================================================================================", color);
    println();
    println();
    print_color("@ ", color);
    print("load_r3\tlr3\tLoads test processes for R3.");
    println();
    print_color("@ ", color);
    print("load_p1\tlp1\tLoads suspended test processes 1.");
    println();
    print_color("@ ", color);
    print("load_p2\tlp2\tLoads suspended test processes 2.");
    println();
    print_color("@ ", color);
    print("load_p3\tlp3\tLoads suspended test processes 3.");
    println();
    print_color("@ ", color);
    print("load_p4\tlp4\tLoads suspended test processes 4.");
    println();
    print_color("@ ", color);
    print("load_p5\tlp5\tLoads suspended test processes 5.");
    println();

    // R5 commands
    println();
    println();
    print_color("===================================================================================================", color);
    println();
    println();

    print_color("@ ", color);
    print("create_mcb\t\tcm\t\tCreates a new MCB.");
    println();
    print_color("@ ", color);
    print("delete_mcb\t\tdm\t\tDeletes an MCB.");
    println();
    print_color("@ ", color);
    print("show_mcb\t\tsm\t\tShows an MCB.");
    println();
    print_color("@ ", color);
    print("show_mcb_free\t\tsmf\t\tShows all free MCBs.");
    println();
    print_color("@ ", color);
    print("show_mcb_alloc\tsma\t\tShows all allocated MCBs.");
    println();
    print_color("@ ", color);
    print("show_mcb_all\t\tsmall\t\tShows all MCBs.");
    println();

    println();
    print_color("===================================================================================================", color); //100 equal signs
    println();
    return 0;
}

int version(void) {
    println();
    print("Version 4.0\nCompilation Date: 3/28/2025");
    return 0;
}

int clear_screen(void) {
    print("\033[2J");
    print("\033[H");
    return 0;
}

char* return_color(void) {

    // return code for each color (for serial.c)
    if (strcmp(color, "green") == 0) {
        return "\x1b[32m";
    }
    else if (strcmp(color, "yellow") == 0) {
        return "\x1b[33m";
    }
    else if (strcmp(color, "blue") == 0) {
        return "\x1b[34m";
    }
    else if (strcmp(color, "magenta") == 0) {
        return "\x1b[35m";
    }
    else if (strcmp(color, "cyan") == 0) {
        return "\x1b[36m";
    }
    else {
        return "\x1b[34m"; // return blue
    }
}
