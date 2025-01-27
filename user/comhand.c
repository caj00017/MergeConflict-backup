// Include required header files
#include <mpx/comhand.h>
#include <mpx/comexec.h>
#include <mpx/device.h>
#include <mpx/serial.h>
#include <sys_req.h>

void comhand(void) {
    while(1) {
        char buf[100] = { 0 };
        sys_req(READ, COM1, buf, sizeof(buf));
        

        /* the following simply echoes the input */
        // sys_req(WRITE, COM1, buf, nread);

        //use this abywhere else 
        /* process the command */
        int status = comexec(buf);
        
        // if the command is a shutdown:
        if (status == 1) {
            // put("Shutting down...");
            return;
        }
        char test[1] = "\n";
        sys_req(WRITE, COM1, test, sizeof(test));
    } 
}
