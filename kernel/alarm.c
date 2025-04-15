#include <mpx/alarm.h>

void alarm_process(void) {

    // get alarm pcb
    pcb* alarm = CURRENT_PCB;
    if (alarm == NULL) {
        // alarm PCB not found
        return;
    }

    // ensure alarm params are not NULL
    if (alarm->params == NULL) {
        // alarm parameters not found
        return;
    }
    
    // cast params from void* to alarm_params*
    alarm_params* args = (alarm_params*)alarm->params;

    // get time from params
    char hr_chars[3] = {args->time[0], args->time[1], '\0'}; // first 2 chars = hour value
    char min_chars[3] = {args->time[3], args->time[4], '\0'}; // next 2 chars (skipping ':') = minute value
    char sec_chars[3] = {args->time[6], args->time[7], '\0'}; // last 2 chars = second value

    // convert time to int
    int param_hour = atoi(hr_chars);
    int param_minute = atoi(min_chars);
    int param_second = atoi(sec_chars);

    // repeatedly check the system time
    for (;;) {
        if (get_hour() >= param_hour) {
            if (get_minute() >= param_minute) {
                if (get_second() >= param_second) {
                    // alarm time reached, print message and exit
                    println();
                    print("[ALARM "); print(alarm->name); print("]: "); print(args->message);
                    println();
                    sys_req(EXIT);
                }
            }
        }
        // yield to allow other processes to run if time not reached
        sys_req(IDLE);
    }
}

int create_alarm(char* message, char* time) {

    // create alarm params process
    alarm_params* params = (alarm_params*)sys_alloc_mem(sizeof(alarm_params));

    // copy message and time to params
    strcpy(params->message, message);
    strcpy(params->time, time);

    if (pcb_find(time) != NULL) {
        // alarm already exists, free params and return error
        print("Alarm already exists with that time.");
        sys_free_mem(params);
        return -1;
    }

    // create pcb for alarm
    pcb* alarm = pcb_setup(time, 1, 0, 1, alarm_process, (void*)params);

    // add to ready queue
    pcb_insert(alarm);
    print("Alarm created successfully.");
    
    return 0;

}
