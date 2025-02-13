// Include required header files
#include <mpx/comhand.h>
#include <mpx/comexec.h>
#include <mpx/device.h>
#include <mpx/serial.h>
#include <sys_req.h>

void comhand(void) {
    sys_req(WRITE, COM1, "@ Type 'help' to see list of commands\n", sizeof("@ Type 'help' to see list of commands\n"));
    while(1) {

        // Add the @ before each command
        sys_req(WRITE, COM1, "@ ", sizeof("@ ")); 

        // initialize buffer and read from user
        char buf[100] = { 0 };
        sys_req(READ, COM1, buf, sizeof(buf));
        
        // process the command
        int status = comexec(buf);
        
        // if the command is a shutdown, exit command loop
        if (status == -1) {
            sys_req(WRITE, COM1, "\nShutting down...\n", sizeof("\nShutting down...\n"));
            return;
        }

        // print newline after each command
        char test[1] = "\n";
        sys_req(WRITE, COM1, test, sizeof(test));
    } 
}
