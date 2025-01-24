#ifndef COMEXEC
#define COMEXEC

/*
*   @brief Function for executing commands. Called by comhand().
*   @param buf Buffer containig the current command to be executed.
*   @return Status code indicating shutdown (1), successful command execution (0), or an invalid command (-1).
*/
int comexec(char buf[]);

/*
*   @brief Function for getting the date set by the user. 
*   @return The current date
*/
char* get_date(void);

/*
*   @brief Function for setting the date.
*   @param The date to be set
*/
void set_date(char* date);

/*
*   @brief Function for getting the time set by the user. 
*   @return The current time
*/
char* get_time(void);

/*
*   @brief Function for setting the time.
*   @param time The time to be set
*/
void set_time(char* time);

/*
*   @brief Function which prints a list of each available command.
*/
void help(void);

#endif
