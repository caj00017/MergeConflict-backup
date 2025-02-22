// Include required header files
#include <mpx/comhand.h>
#include <mpx/comexec.h>
#include <mpx/device.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <string.h>

void comhand(void) {
    print_color("@ ","blue");
    print("Type 'help' to see list of commands\n");
    while(1) {
        
        // process the command
        int status = comexec();
        
        // if the command is a shutdown, exit command loop
        if (status == -1) {
            sys_req(WRITE, COM1, "\nShutting down...\n", sizeof("\nShutting down...\n"));
            return;
        }
        if(status != 5) {
            sys_req(WRITE, COM1, "\n", sizeof("\n"));
        }
    } 
}
