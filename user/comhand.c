// Include required header files
#include <mpx/comhand.h>
#include <mpx/comexec.h>
#include <mpx/device.h>
#include <mpx/serial.h>
#include <sys_req.h>

void comhand(void) {
    while(1) {

        sys_req(WRITE, COM1, "@ ", sizeof("@ ")); // Adds the @ before each command

        char buf[100] = { 0 };
        sys_req(READ, COM1, buf, sizeof(buf));
        
        /* the following simply echoes the input */
        // sys_req(WRITE, COM1, buf, nread);
        //use this anywhere else 
        /* process the command */
        int status = comexec(buf);
        
        // if the command is a shutdown:
        if (status == 1) {
            // put("Shutting down...");
            sys_req(WRITE, COM1, "\nShutting down...\n", sizeof("\nShutting down...\n"));
            return;
        }

        char test[1] = "\n";
        sys_req(WRITE, COM1, test, sizeof(test));
    } 
}
