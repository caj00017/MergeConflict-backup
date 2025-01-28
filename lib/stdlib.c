#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int atoi(const char *s)
{
	int res = 0;
	char sign = ' ';

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while ('0' <= *s && *s <= '9') {
		res = res * 10 + (*s - '0');
		s++;

	}

	if (sign == '-') {
		res = res * -1;
	}

	return res;
}

char* itoa(int num, char* str, int base){
	int i = 0;
	bool Negative = false;

	if(num == 0){
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	while(num != 0){
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	if(Negative){
		str[i++] = '-';
	}

	str[i] = '\0';

	int start = 0;
	int end = i - 1;
	while(start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		end--;
		start++;
	}

	return str;

	
}


