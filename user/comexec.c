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

int comexec(char buf[]) {
    
    if (strcmp(buf, "shutdown") == 0 || strcmp(buf, "sd") == 0) {
        int status = shutdown();
        return status;
    }
    if (strcmp(buf, "version") == 0 || strcmp(buf, "v") == 0) {
        version();
        return 0;
    }
    else if (strcmp(buf, "get_date") == 0 || strcmp(buf, "gd") == 0) {
        get_time();
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
    if (strcmp(buf, "help") == 0) {
        help();
        return 0;
    } 
    else {
        sys_req(WRITE, COM1, "\nError: Invalid command.", sizeof("\nError: Invalid command."));
        return -1;
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
            return 1; //exit and end the program
        }
        else if(strcmp(response, "n") == 0 || strcmp(response, "no") == 0 ){
            sys_req(WRITE, COM1, "\n@ Returning to Usual Operations...", sizeof("\n@ Returning to Usual Operations..."));
            return 0; //exit and continue running program
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
    sys_req(WRITE, COM1, "\n@ help\t\tPrints a complete list of each available command.", sizeof("@ help\tPrints a complete list of each available command."));
    sys_req(WRITE, COM1, "\n@ version\tPrints the current version of the program.", sizeof("@ version\tPrints the current version of the program."));
    sys_req(WRITE, COM1, "\n@ get_date\tPrints the current date set by the user.", sizeof("@ get_date\tPrints the current date set by the user."));
    sys_req(WRITE, COM1, "\n@ set_date\tMM/DD/YY\tSets the current date.", sizeof("@ set_date\tMM/DD/YY\tSets the current date."));
    sys_req(WRITE, COM1, "\n@ get_time\tPrints the current time set by the user.", sizeof("@ get_time\tPrints the current time set by the user."));
    sys_req(WRITE, COM1, "\n@ set_time\tHH:MM:SS\tSets the current time.", sizeof("@ set_time\tHH:MM:SS\tSets the current time."));
    sys_req(WRITE, COM1, "\n@ shutdown\tExits the program.", sizeof("\n@ shutdown\tExits the program."));
    return 0;
}

int version(void) {
    char msg[100] = "\nVersion 1.0\nCompilation Date: 2/7/2025";
    sys_req(WRITE,COM1, msg, sizeof(msg));
    return 0;
}

