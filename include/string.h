#ifndef MPX_STRING_H
#define MPX_STRING_H

#include <stddef.h>

/**
 @file string.h
 @brief A subset of standard C library functions.
*/

/**
 Copy a region of memory.
 @param dst The destination memory region
 @param src The source memory region
 @param n The number of bytes to copy
 @return A pointer to the destination memory region
*/
void* memcpy(void * restrict dst, const void * restrict src, size_t n);

/**
 Fill a region of memory.
 @param address The start of the memory region
 @param c The byte to fill memory with
 @param n The number of bytes to fill
 @return A pointer to the filled memory region
*/
void* memset(void *address, int c, size_t n);

/**
 Compares two strings
 @param s1 The first string to compare
 @param s2 The second string to compare
 @return 0 if strings are equal, <0 if s1 is lexicographically before s2, >0 otherwise
*/
int strcmp(const char *s1, const char *s2);

/**
 Returns the length of a string.
 @param s A NUL-terminated string
 @return The number of bytes in the string (not counting NUL terminator)
*/
size_t strlen(const char *s);

/**
 Split string into tokens
 TODO
*/
char* strtok(char * restrict s1, const char * restrict s2);

/*
* @brief Indicates whether str1 contains str2
* @param str1 The outer string to search
* @param str2 The inner string to find
* @return 1 if str2 was found, 0 otherwise
* @authors Tanner Forbes, Chris Jones
*/
int contains(const char *str1, const char *str2);

/*
* @brief Returns a substring beginning at a specific index
* @param str1 The string to be copied from
* @param index The index to begin copying from
* @return The substring produced from str1
* @author Chris Jones
*/
char* substr(char* str1, int index);

/*
* @brief Determines whether a given string is entirely numeric.
* @param str The string to be tested
* @return 0 for false, 1 for true
* @author Chris Jones
*/
int isNumeric(char* str);

/*
*   @brief Function which returns the amount of a specific character within a string.
*   @param str The String to be searched
*   @param c The character to be identified
*   @author Chris Jones
*/
int charCount(char* str, char c);   

#endif
