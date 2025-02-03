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
int get_time();

int comexec(char buf[]) {
    
    // "shutdown" logic
    if (strcmp(buf, "shutdown") == 0 || strcmp(buf, "sd") == 0) {
        sys_req(WRITE, COM1, "\n", sizeof("\n"));
        int first = 1;
        while (1)
        {
            
            char response[100] = { 0 };
            if(first== 1){
                first++;
                sys_req(WRITE, COM1, "@ Are you sure you want to shutdown? (y or n)", sizeof("@ Are you sure you want to shutdown? (y or n)"));
            }

            sys_req(WRITE, COM1, "\n@ ", sizeof("@ "));
            sys_req(READ, COM1, response, sizeof(response));

            if(strcmp(response, "y") == 0 || strcmp(response, "yes") == 0 ){
                return 1;
            }
            else if(strcmp(response, "n") == 0 || strcmp(response, "no") == 0 ){
                return 0;
            }
            else{
                sys_req(WRITE, COM1, "\n", sizeof("\n"));
                sys_req(WRITE, COM1, "@ Please retype your response: (y or n)", sizeof("@ Please retype your response: (y or n)"));
                continue;
            }
        }
        
        
        return 1;
    }
    if (strcmp(buf, "version") == 0 || strcmp(buf, "v") == 0) {
        // "error: implicit declaration of function 'puts' is invalid in C99"
        // another function must be used
        char msg[100] = "\nVersion 1.0\nCompilation Date: 1/27/2025";
        sys_req(WRITE,COM1, msg, sizeof(msg));

        // serial_out(COM1, test, 8); 
        return 0;
    }

    // "get_date" logic
    else if (strcmp(buf, "get_date") == 0 || strcmp(buf, "gd") == 0) {

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
        sys_req(WRITE, COM1, "\nPlease enter the date to be set (MM/DD/YY): ", sizeof("\nPlease enter the date to be set (MM/DD/YY): "));
    }

    // "set_date" logic - not yet implemented (IP: Tanner Forbes)
    else if (contains(buf, "set_date") == 1) {

        // disable interrupts
        cli();

        char* date = substr(buf, 9);

        if (contains(date, "/") == 0 || strlen(date) != 5 || isNumeric(strtok(date, "/")) == 0) {
            sys_req(WRITE, COM1, "\nInvalid date or format. (MM/DD/YY)", sizeof("\nInvalid date or format. (MM/DD/YY)"));
        }
        else {
            // char mth_chars[2] = {date[0], date[1]}; // first 2 chars = month value
            // char day_chars[2] = {date[3], date[4]}; // next 2 chars = day value
            // char year_chars[2] = {date[6], date[7]}; // last 2 chars = year value

            

            outb(0x70, 0x08);



        


        // logic for setting date

        // re-enable interrupts
        sti();

        return 0;
    }
    }

    // "get_time" logic
    else if (strcmp(buf, "get_time") == 0 || strcmp(buf, "gt") == 0) {

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
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

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

        // derive time string from the buffer
        char* time = substr(buf, 9);

        // check for invalid format (invalid time check pending)
        if (charCount(time, ':') != 2 || strlen(time) != 8 || isNumeric(strtok(time, ":")) == 0) {
            sys_req(WRITE, COM1, "\nInvalid time or format. (HH:MM:SS)", sizeof("\nInvalid time or format. (HH:MM:SS)"));
            return -1;
        }
        else {
            char hr_chars[3] = {time[0], time[1], '\0'}; // first 2 chars = hour value
            char min_chars[3] = {time[3], time[4], '\0'}; // next 2 chars (skipping ':') = minute value
            char sec_chars[3] = {time[6], time[7], '\0'}; // last 2 chars = second value

            // retrieve int values for hour, minute, and second
            unsigned int hour = (unsigned int)atoi(hr_chars);
            unsigned int minute = (unsigned int)atoi(min_chars);
            unsigned int second = (unsigned int)atoi(sec_chars);

            if (hour > 23 || hour < 1 || minute > 59 || minute < 0 || second > 59 || second < 0) {
                sys_req(WRITE, COM1, "\nInvalid time or format. (HH:MM:SS)", sizeof("\nInvalid time or format. (HH:MM:SS)"));
                return -1;
            }

            // convert each time value to BCD
            unsigned int hour_BCD = intToBCD(hour);
            unsigned int minute_BCD = intToBCD(minute);
            unsigned int second_BCD = intToBCD(second);

            // write each BCD value to the corresponding register - this does not work
            outb(0x70, 0x04);
            outb(0x71, hour_BCD);

            outb(0x70, 0x02);
            outb(0x71, minute_BCD);

            outb(0x70, 0x00);
            outb(0x71, second_BCD);

            /* The following sys_req WRITE statements are for testing purposes only */
            // sys_req(WRITE, COM1, "\nTime Received: ", sizeof("\nTime Received: "));
            // sys_req(WRITE, COM1, time, 50);

            // sys_req(WRITE, COM1, "\nBuffer: ", sizeof("\nBuffer: "));
            // sys_req(WRITE, COM1, buf, 50);

            sys_req(WRITE, COM1, "\nHour Received: ", sizeof("\tHour Received: "));
            sys_req(WRITE, COM1, hr_chars, 2);

            sys_req(WRITE, COM1, "\nMinute Received: ", sizeof("\nMinute Received: "));
            sys_req(WRITE, COM1, min_chars, 2);

            sys_req(WRITE, COM1, "\nSecond Received: ", sizeof("\nSecond Received: "));
            sys_req(WRITE, COM1, sec_chars, 2);

            sys_req(WRITE, COM1, "\nCurrent Time: ", sizeof("\nCurrent Time: "));
            get_time();
            /*----------------------------------------------------------------------*/

            
        }

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
        sys_req(WRITE, COM1, "\n@ shutdown\tExits the program.", sizeof("\n@ shutdown\tExits the program."));
        return 0;
    } 
    else {
        sys_req(WRITE, COM1, "\nError: Invalid command.", sizeof("\nError: Invalid command."));
        return 2;
    }

    // Invalid command. There may be different logic for this in the future.
    return -1;
}


// this function is for debugging purposes only
int get_time() {
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
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

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
