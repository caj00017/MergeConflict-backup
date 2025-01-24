// Include required header files
#include <mpx/comexec.h>
#include <string.h>
#include <mpx/serial.h>
#include <mpx/device.h>


int comexec(char buf[]) {
    // Code for executing commands

    // Cue giant wall of if statements. There has to be a better way to do this.
    if (strcmp(buf, "shutdown") == 0) {
        return 1;
    }
    if (strcmp(buf, "version") == 0) {
        // "error: implicit declaration of function 'puts' is invalid in C99"
        // another function must be used

        serial_out(COM1, "Version1", 8); 
        return 0;
    }
    if (strcmp(buf, "get_date") == 0) {
        // printf("%s", get_date());
        return 0;
    }
    if (strcmp(buf, "set_date") == 0) {
        // printf("Date set to %s", buf);
        return 0;
    }
    if (strcmp(buf, "get_time") == 0) {
        // printf("%s", get_time());
        return 0;
    }
    if (strcmp(buf, "set_time") == 0) {
        // printf("Time set to %s", buf);
        return 0;
    }
    if (strcmp(buf, "help") == 0) {
        // help();
        return 0;
    } 

    // Invalid command. There may be different logic for this in the future.
    return -1;
}

// The following functions await implementation.
// comexec.c does not compile without partial implementation of these functions.

// char* get_date(void) {
//     // Get date
// }

// void set_date(char* date) {
//     // Set date
// }

// char* get_time(void) {
//     // Get time
// }

// void set_time(char* time) {
//     // Set time
// }

// void help(void) {
//     // Print list of commands
//     return;
// }
