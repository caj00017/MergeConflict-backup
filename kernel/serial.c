#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>

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
	int pos = 0;
	int exit_return = 0;
	int return_length = 0;

	while (sizeof(buffer) < len && exit_return == 0) /* looping while the buffer size is less than the total length */ {
		if(inb(dev + LSR) & 1) /* checks to see if there is a byte to read */ {

			char c = inb(dev); //reads the byte using inb
			const char *c_ptr = &(c);

			if(c == '\177')
			{
				//Check if there is anything to delete
				if(pos == 0){
					inb(dev);
					continue;
				}

				pos--;
				buffer[pos] = '\0';
				for(int i = pos; i< (return_length) ; i++) {
					buffer[i] = buffer[i + 1];
				}
				return_length--;
				// sys_req(WRITE,COM1,"BACKSPACE",9);
				continue;
				
			}

			//Check for Escape Sequence 
			if((c == '\n') || (c == '\r')){  //checks for escape sqeuence and then exits
				return sizeof(buffer);
				exit_return = 1;
			}

			//Check for Special Character
			if(c == '\033'){
				//Assemble Special String Combination
				char special_key[4] = {c, inb(dev), inb(dev),'\0'};

				// sys_req(WRITE,COM1, special_key , 3);

				if(strcmp(special_key,"\033[A") == 0){
					//UP Arrow
					// sys_req(WRITE,COM1, "UP" , 2);
				}
				else if(strcmp(special_key, "\033[B") == 0){
					//Down Arrow
					// sys_req(WRITE,COM1, "DOWN" , 4);
				}
				else if(strcmp(special_key, "\033[C") == 0){
					pos++;
					//Right Arrow
					// sys_req(WRITE,COM1, "RIGHT" , 5);
				}
				else if(strcmp(special_key, "\033[D") == 0){
					pos--;
					//Left Arrow
					// sys_req(WRITE,COM1, "LEFT" , 4);
				}
				else if(strcmp(special_key, "\033[3") == 0){
					inb(dev);
					
					//Delete
					buffer[pos] = '\0';

					for(int i = pos; i< (return_length) ; i++) {
					buffer[i] = buffer[i + 1];
					}
					return_length--;
					
					// sys_req(WRITE,COM1, "DEL" , 3);
				}
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
				return_length++;
				continue;
			}
			
		}
	}

	return sizeof(buffer);
	// insert your code to gather keyboard input via the technique of polling.
	// You must validate each key and handle special keys such as delete, back space, and
	// arrow keys

	// REMOVE THIS -- IT ONLY EXISTS TO AVOID UNUSED PARAMETER WARNINGS
	// Failure to remove this comment and the following line *will* result in
	// losing points for inattention to detail
	// (void)dev; (void)buffer;

	// THIS MUST BE CHANGED TO RETURN THE CORRECT VALUE
	// return (int)len;
}
