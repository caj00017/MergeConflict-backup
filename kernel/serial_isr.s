bits 32
global serial_isr
extern sys_call

serial_isr:

    call sys_call
    iret