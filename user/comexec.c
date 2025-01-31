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

char bcdToChar(unsigned char bcd);

int comexec(char buf[]) {
    // Code for executing commands

    // "shutdown" logic
    if (strcmp(buf, "shutdown") == 0 || strcmp(buf, "sd") == 0) {

        // return 1 indicating comhand loop break
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

    // "get_date" logic
    else if (strcmp(buf, "get_date") == 0) {

        //disabling the NMI bit
        outb(0x70, inb(0x70) | 0x80);  
        char str[100];
        char *date_ptr;

        //creating a slash char to put between the months/days/years
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

    else if (strcmp(buf, "set_date") == 0) {
        sys_req(WRITE, COM1, "\nPlease enter the date to be set (MM/DD/YY): ", sizeof("\nPlease enter the time to be set (MM/DD/YY): "));
    }

    // "set_date" logic - not yet implemented
    else if (contains(buf, "set_date") == 1) {

        // disable interrupts
        cli();

        char* date = substr(buf, 9);

        if (contains(date, "/") == 0 || strlen(date) != 5 || isNumeric(strtok(date, "/")) == 0) {
            sys_req(WRITE, COM1, "\nInvalid time or format. (MM/DD/YY)", sizeof("\nInvalid time or format. (MM/DD/YY)"));
        }
        else {
            char mth_chars[2] = {date[0], date[1]}; // first 2 chars = month value
            char day_chars[2] = {date[3], date[4]}; // next 2 chars = day value
            char year_chars[2] = {date[6], date[7]}; // last 2 chars = year value

            

date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10)); 
            outb(0x70, 0x08);

            char* month_ptr = (atoi())

            outb(0x71, )

        


        // logic for setting date

        // re-enable interrupts
        sti();

        return 0;
    }

    // "get_time" logic
    else if (strcmp(buf, "get_time") == 0) {

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

        // accessing the minute bit
        outb(0x70, 0x02);

            //changing the minute bit from bcd to a char ptr
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));
            sys_req(WRITE, COM1, time_ptr, sizeof(time_ptr));
            sys_req(WRITE, COM1, colon, sizeof(colon));

        // accessing the second bit 
        outb(0x70, 0x00);

            //changing the second bit from bcd to a char ptr
            int seconds = bcdToChar(inb(0x71)) - 48;
            if(seconds < 10){
                sys_req(WRITE, COM1, "0", sizeof("0"));
            }
            time_ptr = (itoa(seconds, str, 10));
            sys_req(WRITE, COM1, time_ptr, sizeof(time_ptr));

        // return 0 indicating success
        return 0;
    }

    else if (strcmp(buf, "set_time") == 0) {
        sys_req(WRITE, COM1, "\nPlease enter the time to be set (HH:MM): ", sizeof("\nPlease enter the time to be set (HH:MM): "));
    }

    // "set_time" logic - not yet implemented (IP - Chris Jones)
    else if (contains(buf, "set_time") == 1) {
       // disable interrupts
        cli();

        char* time = substr(buf, 9);

        if (charCount(time, ':') != 2 || strlen(time) != 8 || isNumeric(strtok(time, ":")) == 0) {
            sys_req(WRITE, COM1, "\nInvalid time or format. (HH:MM)", sizeof("\nInvalid time or format. (HH:MM)"));
        }
        else {
            char hr_chars[2] = {time[0], time[1]}; // first 2 chars = hour value
            char min_chars[2] = {time[3], time[4]}; // next 2 chars (skipping ':') = minute value
            char sec_chars[2] = {time[6], time[7]}; // last 2 chars = second value

            // Compilation error due to unused variables. Implementation WIP. 
            // unsigned int hour = (unsigned int)atoi(hr_chars);
            // unsigned int minute = (unsigned int)atoi(min_chars);

            sys_req(WRITE, COM1, "\nTime Received: ", sizeof("\nTime Received: "));
            sys_req(WRITE, COM1, time, 50);

            sys_req(WRITE, COM1, "\nBuffer: ", sizeof("\nBuffer: "));
            sys_req(WRITE, COM1, buf, 50); 

            sys_req(WRITE, COM1, "\nHour Received: ", sizeof("\tHour Received: "));
            sys_req(WRITE, COM1, hr_chars, 2);

            sys_req(WRITE, COM1, "\nMinute Received: ", sizeof("\nMinute Received: "));
            sys_req(WRITE, COM1, min_chars, 2);

            sys_req(WRITE, COM1, "\nSecond Received: ", sizeof("\nSecond Received: "));
            sys_req(WRITE, COM1, sec_chars, 2);
        }

        /* Testing
        sys_req(WRITE, COM1, "\nReceived time value: ", sizeof("\nReceived time value: "));
        sys_req(WRITE, COM1, time, 50);

        sys_req(WRITE, COM1, "\nBuffer: ", sizeof("\nBuffer: "));
        sys_req(WRITE, COM1, buf, 50); 
        */

        // logic for setting time

        // outb(0x70, 0x08);
        // outb(0x70, 0x06);
        // outb(0x70, 0x09);

        // re-enable interrupts
        sti();

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
