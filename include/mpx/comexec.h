#ifndef COMEXEC
#define COMEXEC

/** 
* @file comexec.h
* @brief Functions for processing and executing commands.
* @author Chris Jones
* @date 2/4/2025
*/

/**
* @brief Function for executing commands. Called by comhand().
* @param buf Buffer containig the current command to be executed.
* @return Status code indicating shutdown (1), successful command execution (0), or an invalid command (-1).
* @authors Tanner Forbes, Chris Jones, Izaak Whetsell, Evan Humphrey
*/
int comexec(char buf[]);

/**
 * @brief Terminates the command handling sequence.
 * @return Status code (1 = shutdown, 0 = remain open).
 * @author Izaak Whetsell, Chris Jones
 */
int shutdown(void);

/**
 * @brief Prints a list of each available command.
 * @return Status code
 * @author Chris Jones
 */
int help(void);

/**
 * @brief Prints the current version.
 * @return Status code
 * @author Chris Jones
 */
int version(void);

/**
 * @brief Function which utilizes the real time clock registers to display the current time. 
 * @return Status code
 * @author Tanner Forbes
 */
int get_time(void);

/**
 * @brief Function which utilizes the real time clock registers to set the current time. 
 * @return Status code
 * @param buf Command string to extract the time from
 * @author Chris Jones
 */
int set_time(char buf[]);

/**
 * @brief Function which utilizes the real time clock registers to get the current date. 
 * @return Status code
 * @author Tanner Forbes
 */
int get_date(void);

/**
 * @brief Function which utilizes the RTC registers to set the current date. 
 * @param buf Command string to extract the date from
 * @return Status code
 * @author Tanner Forbes
 */
int set_date(char buf[]);

#endif
