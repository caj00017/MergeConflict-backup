#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
 Convert an ASCII string to an integer
 @param s A NUL-terminated string
 @return The value of the string converted to an integer
*/
int atoi(const char *s);

/**
 * @brief Function which converts an integer to an ASCII string
 * @param num The integer to be converted
 * @param str Pointer to the location of the new ASCII string
 * @param base Numeral system to use (i.e. base 10, base 2, base 8, etc.)
 * @author Tanner Forbes
 */
char* itoa(int num, char* str, int base);

/**
* @brief Function which converts a BCD value to a char.
* @param bcd The BCD value to be converted to char.
* @return The char value derived from the BCD
* @author Tanner Forbes
*/
char bcdToChar(unsigned char bcd);

/**
* @brief Function which converts an int to a BCD value
* @param num The int value to be converted to BCD
* @return the BCD value derived from the int 
* @author Chris Jones
*/
unsigned int intToBCD(unsigned int num);

#endif
