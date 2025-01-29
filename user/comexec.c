// Include required header files
#include <mpx/comexec.h>
#include <string.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <sys_req.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <stdlib.h>

// Implementation for comexec
int comexec(char buf[]) {

    // "shutdown" logic
    if (strcmp(buf, "shutdown") == 0) {

        // return 1 indicating comhand loop break
        return 1;
    }

    // "version" logic
    else if (strcmp(buf, "version") == 0) {

        // Write the current version to the console
        char msg[100] = "\nVersion 1.0\nCompilation Date: 1/27/2025";
        sys_req(WRITE,COM1, msg, sizeof(msg));

        // return 0 indicating successful command execution
        return 0;
    }

    // "get_date" logic
    else if (strcmp(buf, "get_date") == 0) {
        outb(0x70, inb(0x70) | 0x80);
        char str[100];
        char *date_ptr;
        char slash[1];
        slash[0] = '/';
        outb(0x70, 0x08);
        date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));
        sys_req(WRITE, COM1, date_ptr, sizeof(date_ptr));
        sys_req(WRITE, COM1, slash, sizeof(slash));
        outb(0x70, 0x07);
        date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));
        sys_req(WRITE, COM1, date_ptr, sizeof(date_ptr));
        sys_req(WRITE, COM1, slash, sizeof(slash));
        outb(0x70, 0x09);
        date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));
        sys_req(WRITE, COM1, date_ptr, sizeof(date_ptr));
        // printf("%s", get_date());
        return 0;
    }

    // "set_date" logic - not yet implemented
    else if (strcmp(buf, "set_date") == 0) {
        cli();
        outb(COM1+0x70, 0x0A);
        // printf("Date set to %s", buf);
        return 0;
    }

    // "get_time" logic - not yet implemented
    else if (strcmp(buf, "get_time") == 0) {
        // printf("%s", get_time());
        return 0;
    }

    // "set_time" logic - not yet implemented 
    else if (strcmp(buf, "set_time") == 0) {
        // printf("Time set to %s", buf);
        return 0;
    }

    // "help" logic
    else if (strcmp(buf, "help") == 0) {

        // Print list of each command
        sys_req(WRITE, COM1, "\n@ help\t\tPrints a complete list of each available command.", sizeof("@ help\tPrints a complete list of each available command."));
        sys_req(WRITE, COM1, "\n@ version\tPrints the current version of the program.", sizeof("@ version\tPrints the current version of the program."));
        sys_req(WRITE, COM1, "\n@ get_date\tPrints the current date set by the user.", sizeof("@ get_date\tPrints the current date set by the user."));
        sys_req(WRITE, COM1, "\n@ set_date\tSets the current date.", sizeof("@ set_date\tSets the current date."));
        sys_req(WRITE, COM1, "\n@ get_time\tPrints the current time set by the user.", sizeof("@ get_time\tPrints the current time set by the user."));
        sys_req(WRITE, COM1, "\n@ set_time\tSets the current time.", sizeof("@ set_time\tSets the current time."));
        
        // return 0 indicating success
        return 0;
    } 

    // logic for invalid commands
    else {

        // print error message
        sys_req(WRITE, COM1, "\nError: Invalid command.", sizeof("\nError: Invalid command."));

        // return 2 indicating failure
        return 2;
    }

    // Unreachable code. comexec.c will not compile without this line.
    return -1;
}

// Implementation for bcdToChar
char bcdToChar(unsigned char bcd){
    int decimal = (bcd >> 4) * 10 + (bcd & 0x0F);
    return decimal + '0';
}
