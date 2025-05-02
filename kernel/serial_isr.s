bits 32
global serial_isr
extern serial_interrupt

serial_isr:
    pushfd               ; save EFLAGS
    pusha                ; save EAX–EDI

    call serial_interrupt

    popa                 ; restore EAX–EDI
    popfd                ; restore EFLAGS
    iret                 ; return from interrupt
