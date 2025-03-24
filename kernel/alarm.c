#include <mpx/alarm.h>

int alarm_process(void* params) {

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
                    return 0;
                }
            }
        }
        sys_req(IDLE);
    }
}

int create_alarm(char* message, char* time) {
    // create alarm process

    // process should be idle before the specified time

    // if the current time matches the requested time or later, display the message and exit the process

}