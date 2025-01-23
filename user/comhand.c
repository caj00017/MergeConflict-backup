// Include required header files
#include "mpx/comhand.h";
#include "mpx/comexec.h";
#include "mpx/device.h";
#include "sys_req.h";

void comhand(void) {
    for (/* something */) {
        char buf[100] = { 0 };
        int nread = sys_req(READ, COM1, buf, sizeof(buf));

        /* the following simply echoes the input */
        sys_req(WRITE, COM1, buf, nread);

        /* process the command */
        int status = comexec(buf);

        // if the command is a shutdown:
        if (status == 1) {
            puts("Shutting down...");
            return;
        }
    } 
}