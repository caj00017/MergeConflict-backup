bits 32
global sys_call_isr
extern sys_call			; The C function that sys_call_isr will call

;;; System call interrupt handler.

 sys_call_isr:
	
	;pushing registers onto the stack into reverse order
	push SS
	push GS
	push FS
	push ES
	push DS
	push EBP
	push EDI
	push ESI
	push EDX
	push ECX
	push EBX
	push EAX
	push ESP

	;calling sys_call and then setting ESP to the return value
	call sys_call
	mov ESP, EAX

	;popping all registers in order and returning
	pop EAX
	pop EBX
	pop ECX
	pop EDX
	pop ESI
	pop EDI
	pop EBP
	pop DS
	pop ES
	pop FS
	pop GS
	pop SS
	iret
