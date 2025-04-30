#ifndef MPX_SERIAL_H
#define MPX_SERIAL_H

#define MAX_RING_BUFFER_SIZE 128

#include <mpx/interrupts.h>
#include <mpx/comexec.h>
#include <mpx/device.h>
#include <mpx/io.h>

#include <sys_req.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stddef.h>

typedef struct dcb {

    device dev_address; // enum corresponding to the device address (COM1, COM2, COM3, COM4)
    int dev; // device number (0-3 for COM1-COM4)
    int open; // 0 closed, 1 open
    int status; // 0 idle, 1 reading, 2 writing
    int* event_flag; // for i/o completion signaling (0 = start, 1 = complete)

    char* input_buf; // pointer to input buffer
    size_t input_len; // number of characters to read
    size_t input_count; // number of characters read

    char* output_buf; // pointer to output buffer
    size_t output_len; // number of characters to write
    size_t output_count; // number of characters written

    char ring_buffer[MAX_RING_BUFFER_SIZE]; // ring buffer for preloading chars before a read
    int ring_start; // index of the head of the ring buffer
    int ring_end; // index of the tail of the ring buffer
    int ring_count; // # of characters in the ring buffer
    
    struct iocb *queue_head; // to track i/o requests waiting to use this device
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

extern dcb* dcb_list[4];

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

/**
 * @brief Opens a serial port for communication
 * @param dev The serial port to open (COM1, COM2, COM3, or COM4)
 * @param speed The baud rate for the serial port (e.g., 9600, 19200)
 * @return 0 on success, non-zero on failure
 * @author Chris Jones
 */
int serial_open(device dev, int speed);

/**
 * @brief Closes a serial port
 * @param dev The serial port to close (COM1, COM2, COM3, or COM4)
 * @return 0 on success, non-zero on failure
 * @author Chris Jones
 */
int serial_close(device dev);

int serial_read(device dev, char *buf, size_t len);

int serial_write(device dev, char *buf, size_t len);

void serial_interrupt(void);

void serial_input_interrupt(dcb* dev);

void serial_output_interrupt(dcb* dev);

/**
 * @brief Get the DCB for a given device number
 * @param devno The device number (0-3 for COM1-COM4)
 * @return Pointer to the DCB structure for the device
 * @author Chris Jones
 */
dcb* get_dcb(int devno);

/**
 * @brief Get the IRQ number for a given device number
 * @param devno The device number (0-3 for COM1-COM4)
 * @return The IRQ number for the device
 * @author Chris Jones
 */
int get_irq(int devno);

dcb* get_dcb(int dev);

iocb* iocb_setup(struct pcb* current_pcb, char* new_buffer, size_t new_length, int op_code);

void iocb_clear(struct iocb* cur_iocb);

#endif
