#ifndef MPX_SERIAL_H
#define MPX_SERIAL_H

#include <stddef.h>
#include <mpx/device.h>

typedef struct dcb {
    int dev;
    int open; //0 = closed, 1 = open
    int status; //0 = idle, 1 = reading, 2 = writing
    int* event_flag;
    char* input_buf;
    size_t input_len;
    size_t input_count;
    char* output_buf;
    size_t output_len;
    size_t output_count;
    char ring_buffer[128];
    int ring_start;
    int ring_end;
    int ring_count;
    struct iocb *queue_head;
} dcb;

typedef struct iocb {
    struct pcb* process;
    int operation;
    char *buffer;
    size_t length;
    size_t transferred;
    int event_flag;
    struct iocb* next;
} iocb;

/** 
    @brief Character Code for Up Arrow Key
*/
#define UP_ARROW "\033[A"

/** 
    @brief Character Code for Down Arrow Key
*/
#define DOWN_ARROW "\033[B"

/** 
    @brief Character Code for Right Arrow Key
*/
#define RIGHT_ARROW "\033[C"

/** 
    @brief Character Code for Left Arrow Key
*/
#define LEFT_ARROW "\033[D"

/** 
    @brief Character Code for Delete Key
*/
#define DELETE_KEY "\033[3"

/** 
    @brief Character Code for Backspace Key
*/
#define BACKSPACE '\177'

/** 
    @brief Character Code for Enter Newline Key
*/
#define NEWLINE '\n'

/** 
    @brief Character Code for Enter Carrage Return Key
*/
#define CARRAGE_RETURN '\r'

/** 
    @brief Character Code for Escape Sequence
*/
#define ESCAPE_SEQUENCE '\033'

/**
 @file mpx/serial.h
 @brief Kernel functions and constants for handling serial I/O
*/

/**
 Initializes devices for user input and output
 @param device A serial port to initialize (COM1, COM2, COM3, or COM4)
 @return 0 on success, non-zero on failure
*/
int serial_init(device dev);

/**
 Writes a buffer to a serial port
 @param device The serial port to output to
 @param buffer A pointer to an array of characters to output
 @param len The number of bytes to write
 @return The number of bytes written
*/
int serial_out(device dev, const char *buffer, size_t len);

/**
 Reads a string from a serial port
 @param device The serial port to read data from
 @param buffer A buffer to write data into as it is read from the serial port
 @param count The maximum number of bytes to read
 @return The number of bytes read on success, a negative number on failure
*/   		   

int serial_poll(device dev, char *buffer, size_t len);

/**
 * Refreshes the buffer in the terminal window after a key is pressed
 * @param buffer The buffer to print to the terminal window
 * @param buf_length The length of the buffer to be printed to the terminal window
 * @param pos The position of the cursor in the terminal window
 * @author Evan Humphrey
 */
void buffer_refresh(char *buffer, int buf_length, int pos);

int serial_open(device dev, int speed);

int serial_close(device dev);

int serial_read(device dev, char *buf, size_t len);

int serial_write(device dev, const char *buf, size_t len);

void serial_interrupt(device dev);

void serial_input_interrupt(device dev);

void serial_output_interrupt(device dev);

#endif
