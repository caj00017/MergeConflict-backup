#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>
#include <mpx/serial.h>

//Function protos
void buffer_refresh(char *buffer, int buf_length, int pos);

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

static int initialized[4] = { 0 };

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}

int serial_poll(device dev, char *buffer, size_t len)
{
	//The pos variable keeps track of the index position of the cursor in the buffer while being displayed in the terminal window
	int pos = 0;
	int exit_return = 0;
	//Size of the buffer
	int entry_length = 0;

	while (sizeof(buffer) < len && exit_return == 0) /* looping while the buffer size is less than the total length */ {
		if(inb(dev + LSR) & 1) /* checks to see if there is a byte to read */ {

			char c = inb(dev); //reads the byte using inb
			const char *c_ptr = &(c);

			// checking for a backspace key
			if(c == BACKSPACE)
			{
				//Check if there is anything to delete
				if(pos == 0){
					continue;
				}
				
				//Adjust the position of the cursor when we delete a char from the buffer
				pos--;
				//Add a null terminator to the buffer to delete the last char deleted from the buffer
				buffer[pos] = '\0';

				//Loop through the buffer and shift all characters to the left to fill the gap of the deleted char if it's not on the end of the buffer
				for(int i = pos; i < (entry_length-1) ; i++) {
					buffer[i] = buffer[i + 1];
				}
				
				//Adjust the size of the buffer
				entry_length--;

				//Keep buffer updated for each button pressed
				buffer_refresh(buffer, entry_length, pos);
				//Continue back to the top of the while loop to check for all possibilites again
				continue;
				
			}

			//Check for Escape Sequence (Enter key)
			if((c == CARRAGE_RETURN) || (c == NEWLINE)){  //checks for escape sqeuence and then exits
				//If we hit the enter key, we want to return the length of the buffer to the command handler
				return sizeof(buffer);
				exit_return = 1;
			}

			//Check for Special Character (Arrow Keys, Delete Key)
			if(c == ESCAPE_SEQUENCE){
				
				//Retrieves addtional characters for matching string
				while(!(inb(dev + LSR) & 1));
				char d = inb(dev);
				while(!(inb(dev + LSR) & 1));
				char e = inb(dev);

				//Assemble Special String Combination
				char special_key[4] = {c, d, e,'\0'};

				// sys_req(WRITE,COM1, special_key , 3);

				//Up Arrow key
				if(strcmp(special_key, UP_ARROW) == 0){

					//make these remember the last few (3) commands that we typed
				}
				//Down Arrow key
				else if(strcmp(special_key, DOWN_ARROW) == 0){

					//make these remember the last few (3) commands that we typed
				}
				//Right Arrow key
				else if(strcmp(special_key, RIGHT_ARROW) == 0){
					//if the cursor tries to go past the end of the buffer, don't let it add to the position counter
					if(pos < entry_length) {
						//If we can go further right, increment the position counter
						pos++;
						//escape sequence to move cursor right (which prints a character to the terminal to move the cursor right)
						sys_req(WRITE, COM1, "\033[1C", sizeof("\033[1D"));
					}
				}
				//Left Arrow key
				else if(strcmp(special_key, LEFT_ARROW) == 0){
					//if the cursor tries to go past the beginning of the buffer, don't let it subtract from the position counter
					if(pos > 0) {
						//If we can go further left, decrement the position counter to keep track of cursor position
						pos--;
						//escape sequence to move cursor left (which prints a character to the terminal to move the cursor left)
						sys_req(WRITE, COM1, "\033[1D", sizeof("\033[1D"));
					}
				}
				//Delete key
				else if(strcmp(special_key, DELETE_KEY) == 0){
					//Retrieves additional characters for matching string
					while(!(inb(dev + LSR) & 1));
					inb(dev);
					
					if(pos != entry_length ){
						//Delete and shift character to the right
						buffer[pos] = '\0';
						
						for(int i = pos; i < entry_length - 1 ; i++) {
							buffer[i] = buffer[i + 1];
						}

						//reduce the entry_length by 1 as we deleted a character
						entry_length--;
					}
					

					//Keep buffer updated for each button pressed
					buffer_refresh(buffer, entry_length, pos);
				}
				//Go back to the beginning of the while loop to check for more entry possibilities
				continue;
			}

			//Check for Basic ASCII
			if(atoi(c_ptr) >= 32 || atoi(c_ptr) <= 126){ 
				
				//Check if there is a character where you are inserting
				unsigned i = pos;
				char next_char = buffer[pos];
				char next2_char;
				//Shift all chars to the right of position one index
				while (buffer[i] !='\0' && i< len-1){
					next2_char = buffer[i+1];
					buffer[i+1] = next_char;
					next_char = next2_char;
					i++;
				}
				
				//Return back to pos and print new char
				buffer[pos] = c; //adds the char to the buffer
				outb(dev, buffer[pos]);//prints the char to the terminal
				pos++;  //updates position in buffer
				entry_length++;

				//Keep buffer updated for each button pressed
				buffer_refresh(buffer, entry_length, pos);
				continue;
			}
			
		}
	}

	return sizeof(buffer);
}

void buffer_refresh(char *buffer, int buf_length, int pos) {
	sys_req(WRITE, COM1, "\033[2K\r", sizeof("\033[2K\r")); //"\033" starts in escape sequence, "2K" clears the terminal line, "\r" prints a carraige return to get back to the beginning of the line
	sys_req(WRITE, COM1, "@", sizeof("@")); //the next two statements reprint the beginning two symbols of the terminal line that appear before every command
	sys_req(WRITE, COM1, " ", sizeof(" ")); //^^^
	sys_req(WRITE, COM1, buffer, buf_length); //this reprints the buffer
	
	//This loop moves the cursor back to the position it was at before the buffer was refreshed, since every time that the buffer is reprinted, the cursor is moved back to the front of the line
	for(int i = buf_length; i > pos; i--) {
		sys_req(WRITE, COM1, "\033[1D", sizeof("\033[1D")); //escape sequence to move cursor left
	}
}
