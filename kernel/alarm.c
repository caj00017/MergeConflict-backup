#include <mpx/alarm.h>

void alarm_process(void* params) {

    alarm_params* args = (alarm_params*)params;

    char hr_chars[3] = {args->time[0], args->time[1], '\0'}; // first 2 chars = hour value
    char min_chars[3] = {args->time[3], args->time[4], '\0'}; // next 2 chars (skipping ':') = minute value
    char sec_chars[3] = {args->time[6], args->time[7], '\0'}; // last 2 chars = second value

    int param_hour = atoi(hr_chars);
    int param_minute = atoi(min_chars);
    int param_second = atoi(sec_chars);

    // repeatedly check the system time
    for (;;) {
        if (get_hour() == param_hour) {
            if (get_minute() == param_minute) {
                if (get_second() == param_second) {
                    print(args->message);
                    sys_req(EXIT);
                }
            }
        }
        sys_req(IDLE);
    }
}

int create_alarm(char* message, char* time) {
    // create alarm process
    alarm_params* params = (alarm_params*)sys_alloc_mem(sizeof(alarm_params));
    strcpy(params->message, message);
    strcpy(params->time, time);

    pcb* alarm = pcb_setup("alarm", 1, 1, 1, alarm_process);

    // store the params as the PCB's params
    alarm->params = params;

    // add to ready queue
    pcb_insert(alarm);
    
    return 0;

}
