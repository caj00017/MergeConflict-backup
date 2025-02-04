/*
*   @file comexec.h
*   @author Chris Jones
*   @date 1/29/2025
*/

#ifndef COMEXEC
#define COMEXEC

/*
*   @brief Function for executing commands. Called by comhand().
*   @param buf Buffer containig the current command to be executed.
*   @return Status code indicating shutdown (1), successful command execution (0), or an invalid command (2).
*   @authors Tanner Forbes, Chris Jones, Izaak Whetsell, Evan Humphrey
*/
int comexec(char buf[]);

/*
*   @brief Function which utilizes the real time clock registers to display the current time. 
*   @return Status code
*   @author Tanner Forbes
*/
int get_time();

#endif
