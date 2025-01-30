#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

/**
 @file ctype.h
 @brief A subset of standard C library functions.
*/

/**
 Determine if a character is whitespace.
 @param c Character to check
 @return Non-zero if space, 0 if not space
*/
int isspace(int c);

/*
@brief Determines if a character is a digit.
@param c Character to check
@return 1 if digit, 0 otherwise.
@author Chris Jones
*/
int isdigit(char c);

#endif
