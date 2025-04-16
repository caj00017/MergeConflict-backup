#include <string.h>
#include <stdlib.h>
#include <conversions.h>

char* custom_itoa(int num, char* str, int base){
	
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

	// tens digit = int / 10
	unsigned int tens = num / 10;

	// ones digit = remainder of int / 10
    unsigned int ones = num % 10; 

	// bit shift the tens digit left by 4, combine with ones
    unsigned int bcd = (tens << 4) | ones; 

	// return BCD
    return bcd;
}

int Math_power(int num, int exponent){
	int new_num = 0;
	if(exponent == 0){
		return 1;
	}
	else{
		new_num = num;
		for(int i = 1; i < exponent; i++){
			new_num = new_num * num;
		}
	}
	return new_num;
}

unsigned int Hex2Dec(char *hex, int hex_size){
	char tempchar = 0;
	int tempnum = 0;
	int dec = 0;
	int power = 0;

	for(int i = hex_size - 1; i >= 0; i--){
		tempchar = hex[i];
		if(tempchar == 'f'){
			tempnum = 15;
		}
		else if(tempchar == 'e'){
			tempnum = 14;
		}
		else if(tempchar == 'd'){
			tempnum = 13;
		}
		else if(tempchar == 'c'){
			tempnum = 12;
		}
		else if(tempchar == 'b'){
			tempnum = 11;
		}
		else if(tempchar =='a'){
			tempnum = 10;
		}
		else{
			tempnum = atoi(&tempchar);
		}
		dec = dec + (tempnum * Math_power(16, power));
		power++;
	}

	return dec;
}

char* Dec2Hex(unsigned int dec){
	char hexarr[100];
	char str[100];
	int storenum = 0;
	char storechar;
	char* storechar_ptr;
	int location = 0;

	while(dec > 16){
		storenum = dec % 16;
		dec = dec / 16;
		if(storenum == 15){
			storechar = 'F';
		}
		else if(storenum == 14){
			storechar = 'E';
		}
		else if(storenum == 13){
			storechar = 'D';
		}
		else if(storenum == 12){
			storechar = 'C';
		}
		else if(storenum == 11){
			storechar = 'B';
		}
		else if(storenum == 10){
			storechar = 'A';
		}
		else{
			storechar_ptr = custom_itoa(storenum, str, 10);
			hexarr[location] = *storechar_ptr;
			location++;
			continue;
		}
		hexarr[location] = storechar;
		location++;
	}

	char* hexarr_ptr = hexarr;

	return hexarr_ptr;
	
}

