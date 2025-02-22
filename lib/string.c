#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <sys_req.h>

/* memcpy() and memset() are in core.c */

int strcmp(const char *s1, const char *s2)
{

	// Remarks:
	// 1) If we made it to the end of both strings (i. e. our pointer points to a
	//    '\0' character), the function will return 0
	// 2) If we didn't make it to the end of both strings, the function will
	//    return the difference of the characters at the first index of
	//    indifference.
	while ((*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

size_t strlen(const char *s)
{
	size_t len = 0;
	while (*s++) {
		len++;
	}
	return len;
}

char *strtok(char * restrict s1, const char * restrict s2)
{
	static char *tok_tmp = NULL;
	const char *p = s2;

	//new string
	if (s1 != NULL) {
		tok_tmp = s1;
	}
	//old string cont'd
	else {
		if (tok_tmp == NULL) {
			return NULL;
		}
		s1 = tok_tmp;
	}

	//skip leading s2 characters
	while (*p && *s1) {
		if (*s1 == *p) {
			++s1;
			p = s2;
			continue;
		}
		++p;
	}

	//no more to parse
	if (!*s1) {
		return (tok_tmp = NULL);
	}
	//skip non-s2 characters
	tok_tmp = s1;
	while (*tok_tmp) {
		p = s2;
		while (*p) {
			if (*tok_tmp == *p++) {
				*tok_tmp++ = '\0';
				return s1;
			}
		}
		++tok_tmp;
	}

	//end of string
	tok_tmp = NULL;
	return s1;
}

int strcpy(char* copy, const char* str) {

    // check for invalid pointers
    if (copy == NULL || str == NULL) {
        return 1; // return error for bad pointers
    }

    while (*str != '\0') {
		*copy = *str;  // copy char from str to copy
		copy++;        // move temp to next char
		str++;         // move str to next char
    }

    *copy = '\0';  // add null terminator for copy
    return 0;
}


int contains(const char *str1, const char *str2) {

	// initialize length values for strings
	int str1_len = strlen(str1);
	int str2_len = strlen(str2);

	// initialize increment variable 
	int j = 0;

	// loop for length of str1
	for(int i = 0; i < str1_len; i++) {
		// if the increment variable is the same length as str2, str2 is found.
		if(j == str2_len){
			return 1;
		}
	    
		// if the characters at this index of both strings are the same, increment j.
		if(str1[i] == str2[j]){
			j++;
		}

		// otherwise, set j back to 0
		else {
			j = 0;
		}
	}
	
	
	// if the increment variable is the same length as str2, str2 is found.
	if (j == str2_len) {
		return 1;
	}

	// otherwise, return 1.
	else {
		return 0;
	}
}

char* substr(char* str, int index) {
    
	// get size of substring
	size_t size = strlen(str) - index;

	// allocate memory for substring
	char* substr = sys_alloc_mem(size);
	if (substr == NULL) {
		return "ERROR";
	}

	// copy characters from str to substr
	int j = index;
	for (size_t i = 0; i < size; i++) {
		substr[i] = str[j];
		j++;
	}
	
	// null terminate substr
	substr[size] = '\0';

	// return the substring
	return substr;
}

int isNumeric(char* str) {
	for (size_t i = 0; i < strlen(str); i++) {
		if (isdigit(str[i]) == 0) {
			return 0; // false
		}
	}
	return 1; // true
}

int charCount(char* str, char c) {
	int count = 0;
	for (size_t i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			count++;
		}
	}
	return count;
}

int indexOf(char* str, char searched_char){
	for(size_t i = 0; i < strlen(str); i++){
		if(str[i] == searched_char){
			return i;
		}
	}
	return -1;
}

void print(const char* sentence){
	//Print Sentance
	sys_req(WRITE, COM1, sentence, strlen(sentence));
}

void println(void){
	//Print newline
	sys_req(WRITE, COM1, "\n", strlen("\n"));
}


void print_error(const char* sentence){
	//Print Red Color Code
	sys_req(WRITE, COM1, "\x1b[31m", strlen("\x1b[31m"));
	//Print Error
	sys_req(WRITE, COM1, sentence, strlen(sentence));
	//Print Color Reset
	sys_req(WRITE, COM1, "\x1b[0m", strlen("\x1b[0m"));
}

void print_color(const char* sentence, const char* color_code){
	// Set color code from given parameter
	if(strcmp(color_code, "green") == 0) {
		color_code = "\x1b[32m";
	}
	else if(strcmp(color_code, "yellow") == 0) {
		color_code = "\x1b[33m";
	}
	else if(strcmp(color_code, "blue") == 0) {
		color_code = "\x1b[34m";
	}
	else if(strcmp(color_code, "magenta") == 0) {
		color_code = "\x1b[35m";
	}
	else if(strcmp(color_code, "cyan") == 0) {
		color_code = "\x1b[36m";
	}
	else {
		color_code = "";
	}
	
	//Print Color Code
	sys_req(WRITE, COM1, color_code, strlen(color_code));
	//Print Sentence
	sys_req(WRITE, COM1, sentence, strlen(sentence));
	//Print Color Reset
	sys_req(WRITE, COM1, "\x1b[0m", strlen("\x1b[0m"));
}



char* trim(char * sentence){
	if(sentence == NULL){
		return NULL;
	}

	//Remove whitespace from the front
	while(isspace(*sentence))
	{
		sentence++;
	}

	//Remove whitespace from end of sentence
	char* end = sentence + strlen(sentence) -1;
	while(end > sentence && isspace(*end)){
		*end = '\0';
		end--;
	}

	return sentence;
}

