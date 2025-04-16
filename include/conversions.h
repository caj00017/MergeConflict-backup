/**
 * @file conversions.h
 * @brief Header file containing conversion functions to be implemented in conversions.c.
 * @authors Chris Jones
 */


/**
 * @brief Function which converts an integer to an ASCII string
 * @param num The integer to be converted
 * @param str Pointer to the location of the new ASCII string
 * @param base Numeral system to use (i.e. base 10, base 2, base 8, etc.)
 * @author Tanner Forbes
 */
char* custom_itoa(int num, char* str, int base);

/**
* @brief Function which converts a BCD value to a char.
* @param bcd The BCD value to be converted to char.
* @return The char value derived from the BCD
* @author Tanner Forbes
*/
char bcdToChar(unsigned char bcd);

/**
* @brief Function which converts an int to a BCD value
* @param num The unsigned int value (0-99 only) to be converted to BCD
* @return the BCD value derived from the int 
* @author Chris Jones
*/
unsigned int intToBCD(unsigned int num);

/**
 * @brief Function which multiplies a num by itself however many times specified
 * @param num The integer to be multiplied together
 * @param exponent The amount of times the num is multiplied together
 * @return The product after being multiplied however many times
 * @author Tanner Forbes
 */
int Math_power(int num, int exponent);

/**
 * @brief Function that converts from Hex to Decimal
 * @param hex A char pointer that points to the beginning of the hex char array
 * @param hex_size The size of the hex char array
 * @return The decimal equivalent of the hex number
 * @author Tanner Forbes
 */
unsigned int Hex2Dec(char *hex, int hex_size);

/**
 * @brief Function that converts from Decimal to Hex
 * @param dec An unsigned int that will be converted to hex
 * @return A char pointer that points to the beginning of the converted hex char array
 * @author Tanner Forbes
 */
char* Dec2Hex(unsigned int dec);

