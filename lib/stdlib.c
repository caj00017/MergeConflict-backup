#include <stdlib.h>
#include <ctype.h>

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
	
	//Initilization of a int variable to keep track of the position within the char array
	int pos = 0;

	//checking to see if the num to convert is a 0
	if(num == 0){
		str[pos++] = '0';
		str[pos] = '\0';
		return str;
	}

	//looping while the num is not a 0
	while(num != 0){

		//Modding the num by the base in order to get the remainder
		int rem = num % base;
		str[pos++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';

		//Dividing the number by the base to get rid of the least significant digit
		num = num / base;
	}

	//Once out of the loop add a null terminator to the end of the char array
	str[pos] = '\0';

	int start = 0;
	int end = pos - 1;
	
	//inverting the char array to get the correct ASCII string
	while(start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		end--;
		start++;
	}

	//returning the passed str parameter
	return str;

	
}

// Implementation for bcdToChar
char bcdToChar(unsigned char bcd){
	
	//converting the given bcd to decimal by bit shifting then multiplying then adding
    int decimal = (bcd >> 4) * 10 + (bcd & 0x0F);

	//returning the int plus a char to turn the given int into a char
    return decimal + '0';
}

// Implementation for intToBCD
unsigned int intToBCD(unsigned int num) {
    unsigned int ones = 0;
    unsigned int tens = 0;
    unsigned int temp = 0;

    ones = num % 10; 
	temp = num / 10;
    tens = (temp << 4) | ones; 
    return tens;
}


