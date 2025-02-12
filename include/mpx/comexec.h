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

#endif
