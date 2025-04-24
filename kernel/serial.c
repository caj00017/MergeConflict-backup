#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>
#include <mpx/serial.h>
#include <mpx/comexec.h>
#include <memory.h>
#include <mpx/interrupts.h>

// declare assembly stub
extern void serial_isr(void);

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

// global variables for each device
struct dcb COM1_DCB;
struct dcb COM2_DCB;
struct dcb COM3_DCB;
struct dcb COM4_DCB;

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

	while (sizeof(buffer) < len && exit_return == 0 && pos < (int)len) /* looping while the buffer size is less than the total length */ {
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

				//Up Arrow key
				if(strcmp(special_key, UP_ARROW) == 0){

					//make these remember the last few (5) commands that we typed
				}
				//Down Arrow key
				else if(strcmp(special_key, DOWN_ARROW) == 0){

					//make these remember the last few (5) commands that we typed
				}
				//Right Arrow key
				else if(strcmp(special_key, RIGHT_ARROW) == 0){
					//if the cursor tries to go past the end of the buffer, don't let it add to the position counter
					if(pos < entry_length) {
						//If we can go further right, increment the position counter
						pos++;
						//escape sequence to move cursor right (which prints a character to the terminal to move the cursor right)
						serial_out(COM1, "\033[1C", sizeof("\033[1D"));
					}
				}
				//Left Arrow key
				else if(strcmp(special_key, LEFT_ARROW) == 0){
					//if the cursor tries to go past the beginning of the buffer, don't let it subtract from the position counter
					if(pos > 0) {
						//If we can go further left, decrement the position counter to keep track of cursor position
						pos--;
						//escape sequence to move cursor left (which prints a character to the terminal to move the cursor left)
						serial_out(COM1, "\033[1D", sizeof("\033[1D"));
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

	char* color = return_color(); // get the color that the user sets in comexec
	serial_out(COM1, "\033[2K\r", sizeof("\033[2K\r")); //"\033" starts in escape sequence, "2K" clears the terminal line, "\r" prints a carraige return to get back to the beginning of the line
	serial_out(COM1, color, sizeof("\x1b[34m")); //changes the color of the @ symbol to set color (using size for blue, all color codes are the same size)
	serial_out(COM1, "@", sizeof("@")); //the next two statements reprint the beginning two symbols of the terminal line that appear before every command
	serial_out(COM1, " ", sizeof(" ")); //^^^
	serial_out(COM1, "\x1b[0m", sizeof("\x1b[0m")); //resets the color of the terminal line to the default color
	serial_out(COM1, buffer, buf_length); //this reprints the buffer
	
	//This loop moves the cursor back to the position it was at before the buffer was refreshed, since every time that the buffer is reprinted, the cursor is moved back to the front of the line
	for(int i = buf_length; i > pos; i--) {
		serial_out(COM1, "\033[1D", sizeof("\033[1D")); //escape sequence to move cursor left
	}
}

int serial_open(device dev, int speed)
{
	// int dno = serial_devno(dev);
	// if (dno == -1) {
	// 	return -1;
	// }
	// if (initialized[dno] == 0) {
	// 	serial_init(dev);
	// }
	// return 0;

	/** The following documentation is from the document 
	* entitled "Module R6: Interrupt Driven I/O" 
	*/

	/**
	* 1. Ensure that the parameters are valid,
	* and that the device is not currently open.
	*/

	if (	
		speed != 110
		&& speed != 150
		&& speed != 300
		&& speed != 600
		&& speed != 1200
		&& speed != 2400
		&& speed != 4800
		&& speed != 9600
		&& speed != 19200
	) {
		return -102; // verify baud rate is valid
	}

	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1; // verify device is valid, this status code may be wrong
	}

	if (initialized[dno] != 1) {
		return -103; // verify device is not open
	}

	/**
	* 2. Initialize the DCB. 
	* In partciular, this should include:
	*/

		dcb* new_dcb = (dcb*)sys_alloc_mem(sizeof(dcb));

	 	// indicating that the device is open
		new_dcb->open = 1;
		
	 	// setting the event flag to 0
		new_dcb->event_flag = 0;

	 	// setting the initial device status to idle
		new_dcb->status = 0;

	 	// initializing the ring buffer parameters

		new_dcb->input_buf = NULL;
		new_dcb->input_len = 0;
		new_dcb->input_count = 0;

		new_dcb->output_buf = NULL;
		new_dcb->output_len = 0;
		new_dcb->output_count = 0;

		memset(new_dcb->ring_buffer, 0, sizeof(new_dcb->ring_buffer)); // init ring buffer to 0
		new_dcb->ring_start = 0;
		new_dcb->ring_end = 0;
		new_dcb->ring_count = 0;

		new_dcb->queue_head = NULL; // initialize the queue head to NULL
	
	/**
	* 3. Install the new handler in the interrupt vector.
	*/

	int vector;
	switch (dno) {
		case 0:
		case 2:
			vector = 0x24; // IRQ 4
			break;
		case 1:
		case 3:
			vector = 0x23; // IRQ 3
			break;
		default:
			return -1; // invalid device number
	}

	idt_install(vector, (void (*)(void*))serial_isr);

	/**
	* 4. Compute the required baud rate divisor.
	*/

	// unsigned short = 16 bit
	unsigned short baud_rate_div = 115200 / (long)speed;

	/**
	* 5. Store the value 0x80 in the Line Control Register.
	* This allows the first two port addresses to access the
	* Baud Rate Divisor register.
	*/

	outb(dev + LCR, 0x80);	//set line control register, from serial_init()

	/**
	* 6. Store the high order and low order bytes of the
	* baud rate divisor into the MSB and LSB registers,
	* respectively.
	*/

	// unsigned char = 8 bit
	unsigned char lsb = baud_rate_div & 0xFF;	// least significant byte
	unsigned char msb = (baud_rate_div >> 8) & 0xFF;	// most significant byte
	outb(dev + DLL, lsb);	//set bsd least sig bit
	outb(dev + DLM, msb);	//brd most significant bit

	/**
	* 7. Store the value 0x03 in the LCR.
	* This sets the line characteristics to 8 data bits,
	* 1 stop bit, and no parity. It also restores normal
	* functioning of the first two ports.
	*/

	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop

	/**
	* 8. Enable the appropriate level in the PIC mask register.
	*/

	int irq;
	switch (dno) {
		case 0:
		case 2:
			irq = 4; 
			break;
		case 1:
		case 3:
			irq = 3; 
			break;
		default:
			return -1; // invalid device number
	}

	/* Adapted from example code in Section 3.6 */
	cli();
	int mask = inb(0x21);
	mask &= ~(1 << irq); // set bit at index = irq to 0
	outb(0x21, mask);	// enable IRQ 3 or 4 in PIC
	sti();

	/**
	* 9. Enable overall serial port interrupt by storing the
	* value 0x08 in the Modem Control register
	*/
	outb(dev + MCR, 0x08);

	/**
	* 10. Enable input ready interrupts only by storing the value
	* 0x01 in the Interrupt Enable register.
	*/
	outb(dev + IER, 0x01);

	initialized[dno] = 1; // mark the device as initialized
	return 0; // success
}

int serial_close(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	initialized[dno] = 0;
	return 0;
}

int serial_read(device dev, char *buf, size_t len)
{
	//dev could be COM1 - COM4
	dcb* current_dev;
	//1) validate the supplied params
	//a) check that the device number is valid (COM1 - COM4)
	switch (serial_devno(dev)) {
		case COM1:
			current_dev = &COM1_DCB;
			break;
		case COM2:
			current_dev = &COM2_DCB;
			break;
		case COM3:
			current_dev = &COM3_DCB;
			break;
		case COM4:
			current_dev = &COM4_DCB;
			break;
		default:
			return -1; // Invalid device number
	}

	//b) check that the device is open (initialized[dno] == 1)
	if(initialized[serial_devno(dev)] == 0) {
		//if the device is not open, return -301
		return -301;
	}
	//c) check that the buffer is not NULL
	if(buf == NULL) {
		//if the buffer is null, return -302
		return -302;
	}
	//d) check that the indicated length is not greater than the size of the buffer or less than zero
	if(len < 0 || len > MAX_RING_BUFFER_SIZE) {
		//if the length is less than zero or greater than the size of the buffer, return -303
		return -303;
	}

	//2) ensure that the status of the port is idle
	//a) check that the status is not reading or writing (status == IDLE)
	if(current_dev->status != IDLE) {
		//if the status is not idle, return -304
		return -304;
	}
	//b) check that the input buffer is not NULL (input_buf == NULL)
	if(current_dev->input_buf == NULL) {
		//if the input buffer is null, return -305
		return -305;
	}


	//3) Initialize the input buffer variables (not the ring buffer!) and set the status to reading
	//a) set the input buffer to the supplied buffer (input_buf = buf)
	current_dev->input_buf = buf;
	//b) set the input length to the supplied length (input_len = len)
	current_dev->input_len = len;
	//c) set the input count to zero (input_count = 0)
	current_dev->input_count = 0;
	//d) set the status to reading (status = READING)
	current_dev->status = 1;

	//4) Clear the caller’s event flag
	//a) check that the event flag is not NULL (event_flag == NULL)
	if(current_dev->event_flag == NULL) {
		//if the event flag is null, return -306
		return -306;
	}
	//b) clear the event flag (event_flag = 0)
	current_dev->event_flag = 0;

	//5) Copy characters from the ring buffer to the requestor’s buffer, until the ring buffer is emptied, the
	//requested count has been reached, or a new-line (ENTER) code has been found. The copied characters
	//should, of course, be removed from the ring buffer. Either input interrupts or all interrupts should be
	//disabled during the copying
	//a) check that the ring buffer is not empty (ring_count == 0)
	if(current_dev->ring_count == 0) {
		//if the ring buffer is empty, return -307
		return -307;
	}
	//b) check that the requested count has not been reached (input_count == input_len)
	if(current_dev->input_count == current_dev->input_len) {
		//if the requested count has been reached, return -308
		return -308;
	}
	//c) check that a new-line code has not been found (new_line == 0)
	if(current_dev->input_buf[current_dev->input_count] != NEWLINE) {
		//if a new-line code has been found, return -309
		return -309;
	}	
	

	//6) If more characters are needed, return. If the block is complete, continue with step 7
	

	//7) Reset the DCB status to idle, set the event flag, and return the actual count to the requestor’s variable

	//Notice that it is not necessary for serial read() to enable or disable input interrupts, except while
	//the ring buffer is being accessed. These are always enabled while the port is open. However, we must not
	//allow the process of removing characters from the ring buffer to be interrupted by an attempt to put a new
	//character in

	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	return serial_poll(dev, buf, len);
}

int serial_write(device dev, const char *buf, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	return serial_out(dev, buf, len);
}

void serial_interrupt(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return;
	}
	serial_input_interrupt(dev);
}

void serial_input_interrupt(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return;
	}
	serial_read(dev, NULL, 0);
}

void serial_output_interrupt(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return;
	}
	serial_write(dev, NULL, 0);
}
