#include <ctype.h>

int isspace(int c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\f' || c == '\t' || c == '\v');
}

int isdigit(char c) {
	if (c >= '0' && c <= '9') {
		return 1; // true
	}
	else {
		return 0; // false
	}
}
