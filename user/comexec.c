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

int get_time(void) {
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
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 53, str, 10)); // -5 offset

            // write the hour to the terminal (size checking)
            if (atoi(time_ptr) <= 9) {
                sys_req(WRITE, COM1, "0", sizeof("0"));
                sys_req(WRITE, COM1, time_ptr, 1);
            }
            else {
                sys_req(WRITE, COM1, time_ptr, 2);
            }

            //writing a colon to the terminal
            sys_req(WRITE, COM1, colon, sizeof(colon));

        // accessing the minute bit
        outb(0x70, 0x02);

            //changing the minute bit from bcd to a char ptr
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

            // write the minute to the terminal (size checking)
            if (atoi(time_ptr) <= 9) {
                sys_req(WRITE, COM1, "0", sizeof("0"));
                sys_req(WRITE, COM1, time_ptr, 1);
            }
            else {
                sys_req(WRITE, COM1, time_ptr, 2);
            }

            sys_req(WRITE, COM1, colon, sizeof(colon));

        // accessing the second bit 
        outb(0x70, 0x00);

            //changing the second bit from bcd to a char ptr
            time_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

            // write the second to the terminal (size checking)
            if (atoi(time_ptr) <= 9) {
                sys_req(WRITE, COM1, "0", sizeof("0"));
                sys_req(WRITE, COM1, time_ptr, 1);
            }
            else {
                sys_req(WRITE, COM1, time_ptr, 2);
            }

        // return 0 indicating success
        return 0;
}

int set_time(char buf[]) {
    
       // disable interrupts
        cli();

        // derive time string from the buffer
        char* time = substr(buf, 9);

        // check for invalid format (invalid time check pending)
        if (charCount(time, ':') != 2 || strlen(time) != 8 || isNumeric(strtok(time, ":")) == 0) {
            sys_req(WRITE, COM1, "\nInvalid set_time format. (set_time HH:MM:SS)", sizeof("\nInvalid set_time format. (set_time HH:MM:SS)"));
            return -1;
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
                sys_req(WRITE, COM1, "\nInvalid time. (set_time [00-23]:[00-59]:[00-59])", sizeof("\nInvalid time. (set_time [00-23]:[00-59]:[00-59])"));
                return -1;
            }

            // retrieve int values for hour, minute, and second
            unsigned int hour = (unsigned int)atoi(hr_chars) + 5; // +5 for EST
            unsigned int minute = (unsigned int)atoi(min_chars);
            unsigned int second = (unsigned int)atoi(sec_chars);

            // convert each time value to BCD
            unsigned int hour_BCD = intToBCD(hour);
            unsigned int minute_BCD = intToBCD(minute);
            unsigned int second_BCD = intToBCD(second);

            // write each BCD value to the corresponding register
            outb(0x70, 0x04);
            outb(0x71, hour_BCD); // hour

            outb(0x70, 0x02);
            outb(0x71, minute_BCD); // minute

            outb(0x70, 0x00);
            outb(0x71, second_BCD); // second

            sys_req(WRITE, COM1, "\nTime set to: ", sizeof("\nTime set to: "));
            get_time();
        }

        // re-enable interrupts
        sti();

        return 0;
}

int get_date(void){

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

            // write the month to the terminal (size checking)
            if (atoi(date_ptr) <= 9) {
                sys_req(WRITE, COM1, date_ptr, 1);
            }
            else {
                sys_req(WRITE, COM1, date_ptr, 2);   
            }

             //writing the month and a slash to the terminal      
            sys_req(WRITE, COM1, slash, sizeof(slash));     

        //accessing the day bit
        outb(0x70, 0x07);

            //changing the bit from bcd to a char ptr                             
            date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

            // write the day to the terminal (size checking)
            if (atoi(date_ptr) <= 9) {
                sys_req(WRITE, COM1, date_ptr, 1);
            }
            else {
                sys_req(WRITE, COM1, date_ptr, 2);   
            }
            
            // write a slash to the terminal
            sys_req(WRITE, COM1, slash, sizeof(slash));

        //accessing the year bit
        outb(0x70, 0x09);  

            //changing the bit from bcd to a char ptr                             
            date_ptr = (itoa(bcdToChar(inb(0x71)) - 48, str, 10));

            //writing the year to the terminal
            sys_req(WRITE, COM1, date_ptr, 2);

    //returning 0 indicates success   
    return 0;
}

int set_date(char buf[]) {
    
        // disable interrupts
        cli();

        //getting the inputted date from the buffer
        char* date = substr(buf, 9);

        //checking to make sure the date is in a valid format
        if (contains(date, "/") == 0 || strlen(date) != 8 || isNumeric(strtok(date, "/")) == 0) {
            sys_req(WRITE, COM1, "\nInvalid set_date format. (set_date MM/DD/YY)", sizeof("\nInvalid set_date format. (set_date MM/DD/YY)"));
        }
        else {
            char mth_chars[3] = {date[0], date[1], '\0'}; // first 2 chars = month value
            char day_chars[3] = {date[3], date[4], '\0'}; // next 2 chars = day value
            char year_chars[5] = {date[6], date[7],'\0'}; // last 2 chars = year value

            unsigned int month = (unsigned int)atoi(mth_chars); //converting the month char to a unsigned int
            unsigned int day = (unsigned int)atoi(day_chars); //converting the day char to a unsigned int
            unsigned int year = (unsigned int)atoi(year_chars); //converting the year char to a unsigned int

            //checking for valid month input
            if(month > 12){
                sys_req(WRITE, COM1, "\nInvalid Month", sizeof("\nInvalid Month"));
                return -1;
            }
            //checking for valid day input for months with 31 days in them
            else if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
                if(day > 31 || day < 1){
                    sys_req(WRITE, COM1, "\nInvalid Day", sizeof("\nInvalid Day"));
                    return -1;
                }
            }
            //checking for valid day input for months with 30 days in them
            else if(month == 4 || month == 6 || month == 9 || month == 11){
                if(day > 30 || day < 1){
                    sys_req(WRITE, COM1, "\nInvalid Day", sizeof("\nInvalid Day"));
                    return -1;
                }
            }
            //checking for valid day input for february during a leap year
            else if(month == 2 && (year % 4) == 0 ){
                if(day > 29 || day < 1){
                    sys_req(WRITE, COM1, "\nInvalid Day", sizeof("\nInvalid Day"));
                    return -1;
                }
            }
            //checking for valid day input for february not during a leap year
            else if(month == 2){
                  if(day > 28 || day < 1){
                    sys_req(WRITE, COM1, "\nInvalid Day", sizeof("\nInvalid Day"));
                    return -1;
                }
            }

            unsigned int month_BCD = intToBCD(month); //converting month int to BCD
            unsigned int day_BCD = intToBCD(day); //converting day int to BCD
            unsigned int year_BCD = intToBCD(year); //converting year int to BCD

            //write each BCD value to the corresponding register
            
            outb(0x70, 0x09); //accessing the year register
            outb(0x71, year_BCD); //writing the year BCD to the month register

            outb(0x70, 0x08); //accessing the month register
            outb(0x71, month_BCD); //writing the month BCD to the day register

            outb(0x70, 0x07); //accessing the day register
            outb(0x71, day_BCD); //writing the day BCD to the year register

            sys_req(WRITE, COM1, "\nDate set to: ", sizeof("\nDate set to: "));
            get_date();

        // re-enable interrupts
        sti();
        }
    return 0;
}
