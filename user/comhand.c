// Include required header files
#include "mpx/comhand.h";

void comhand(void) {
    for (/* something */) {
        char buf[100] = { 0 };
        int nread = sys_req(READ, COM1, buf, sizeof(buf));
        /* the following simply echoes the input */
        sys_req(WRITE, COM1, buf, nread);
        /* process the command */
        if (/* command was shutdown and shutdown is confirmed */) {
            return;
        }
    } 
}