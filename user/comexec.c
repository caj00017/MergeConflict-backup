// Include required header files
#include <mpx/comexec.h>
#include <string.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <sys_req.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <stdlib.h>

char bcdToChar(unsigned char bcd);

int comexec(char buf[]) {
    // Code for executing commands

    // Cue giant wall of if statements. There has to be a better way to do this.
    if (strcmp(buf, "shutdown") == 0) {
        return 1;
    }
    if (strcmp(buf, "version") == 0) {
        // "error: implicit declaration of function 'puts' is invalid in C99"
        // another function must be used
        char msg[100] = "\nVersion 1.0\nCompilation Date: 1/27/2025";
        sys_req(WRITE,COM1, msg, sizeof(msg));

        // serial_out(COM1, test, 8); 
        return 0;
    }
    if (strcmp(buf, "get_date") == 0) {

        //disabling the NMI bit
        outb(0x70, inb(0x70) | 0x80);  
        char str[100];
        char *date_ptr;
        char slash[1];
            slash[0] = '/';
        sys_req(WRITE, COM1, "\n", sizeof("\n"));

        //accessing the month bit
        outb(0x70, 0x08);      

            //changing the bit from bcd to a char ptr                      
            date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));  

             //writing the month and a slash to the terminal
            sys_req(WRITE, COM1, date_ptr, sizeof(date_ptr));         
            sys_req(WRITE, COM1, slash, sizeof(slash));            

        //accessing the day bit
        outb(0x70, 0x07);

            //changing the bit from bcd to a char ptr                             
            date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

            //writing the day and a slash to the terminal
            sys_req(WRITE, COM1, date_ptr, sizeof(date_ptr));
            sys_req(WRITE, COM1, slash, sizeof(slash));

        //accessing the year bit
        outb(0x70, 0x09);  

            //changing the bit from bcd to a char ptr                             
            date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

            //writing the year to the terminal
            sys_req(WRITE, COM1, date_ptr, sizeof(date_ptr));
        
        return 0;
    }
    if (strcmp(buf, "set_date") == 0) {
        cli();
        outb(0x70, 0x08);
        outb(0x70, 0x06);
        outb(0x70, 0x09);
        // printf("Date set to %s", buf);
        return 0;
    }
    if (strcmp(buf, "get_time") == 0) {

        //disabling the NMI bit
        outb(0x70, inb(0x70) | 0x80);
        char str[100];
        char *time_ptr;
        char colon[1];
        colon[0] = ':';
        sys_req(WRITE, COM1, "\n", sizeof("\n"));

        //accessing the hour bit
        outb(0x70, 0x04);

            //changing the hour bit from bcd to a char ptr
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 53, str, 10));

            //writing the hour bit and a colon to the terminal
            sys_req(WRITE, COM1, time_ptr, sizeof(time_ptr));
            sys_req(WRITE, COM1, colon, sizeof(colon));
        outb(0x70, 0x02);

            //changing the minute bit from bcd to a char ptr
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));
            sys_req(WRITE, COM1, time_ptr, sizeof(time_ptr));
            sys_req(WRITE, COM1, colon, sizeof(colon));
        outb(0x70, 0x00);

            //changing the second bit from bcd to a char ptr
            int seconds = bcdToChar(inb(0x71)) - 48;
            if(seconds < 10){
                sys_req(WRITE, COM1, "0", sizeof("0"));
            }
            time_ptr = (itoa(seconds, str, 10));
            sys_req(WRITE, COM1, time_ptr, sizeof(time_ptr));
        // printf("%s", get_time());
        return 0;
    }
    if (strcmp(buf, "set_time") == 0) {
        // printf("Time set to %s", buf);
        return 0;
    }
    if (strcmp(buf, "help") == 0) {
        // Print list of commands
        sys_req(WRITE, COM1, "\n@ help\t\tPrints a complete list of each available command.", sizeof("@ help\tPrints a complete list of each available command."));
        sys_req(WRITE, COM1, "\n@ version\tPrints the current version of the program.", sizeof("@ version\tPrints the current version of the program."));
        sys_req(WRITE, COM1, "\n@ get_date\tPrints the current date set by the user.", sizeof("@ get_date\tPrints the current date set by the user."));
        sys_req(WRITE, COM1, "\n@ set_date\tSets the current date.", sizeof("@ set_date\tSets the current date."));
        sys_req(WRITE, COM1, "\n@ get_time\tPrints the current time set by the user.", sizeof("@ get_time\tPrints the current time set by the user."));
        sys_req(WRITE, COM1, "\n@ set_time\tSets the current time.", sizeof("@ set_time\tSets the current time."));
        return 0;
    } 
    else {
        sys_req(WRITE, COM1, "\nError: Invalid command.", sizeof("\nError: Invalid command."));
        return 2;
    }

    // Invalid command. There may be different logic for this in the future.
    return -1;
}

char bcdToChar(unsigned char bcd){
    int decimal = (bcd >> 4) * 10 + (bcd & 0x0F);
    return decimal + '0';
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
