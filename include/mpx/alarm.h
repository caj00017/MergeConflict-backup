#include <mpx/time.h>
#include <mpx/pcb.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <mpx/sys_call.h>

/**
 * @file alarm.h
 * @authors Chris Jones
 * @brief Contains alarm function prototypes and parameter struct.
 */

typedef struct {
    char message[100]; // the message that the alarm will print
    char time[9]; // 9-character time string (similar to that in time.h) to be compared within alarm_process()
} alarm_params;

/**
 * @brief Function which handles alarm process logic.
 * @authors Chris Jones
 */
void alarm_process(void);

/**
 * @brief Function which creates a new alarm process.
 * @authors Chris Jones
 */
int create_alarm(char* message, char* time);
