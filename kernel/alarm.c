#include <mpx/alarm.h>

void alarm_process(void) {

    print("Starting alarm process...\n");

    print("Getting alarm PCB...\n");
    pcb* alarm = CURRENT_PCB;
    if (alarm == NULL) {
        print("Alarm process not found.\n");
        return;
    }
    print("Alarm PCB found.\n");

    print("Fetching parameters...\n");
    if (alarm->params == NULL) {
        print("Alarm parameters not found.\n");
        return;
    }
    print("Alarm parameters found.\n");
    alarm_params* args = (alarm_params*)alarm->params;

    print("Fetching time...\n");
    char hr_chars[3] = {args->time[0], args->time[1], '\0'}; // first 2 chars = hour value
    char min_chars[3] = {args->time[3], args->time[4], '\0'}; // next 2 chars (skipping ':') = minute value
    char sec_chars[3] = {args->time[6], args->time[7], '\0'}; // last 2 chars = second value

    print("Converting time...\n");
    int param_hour = atoi(hr_chars);
    int param_minute = atoi(min_chars);
    int param_second = atoi(sec_chars);

    print("Entering system time check loop...\n");
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

    char* alarm_name = strcat("alarm_", time);

    pcb* alarm = pcb_setup(alarm_name, 1, 1, 1, alarm_process, (void*)params);

    // add to ready queue
    pcb_insert(alarm);
    
    return 0;

}
