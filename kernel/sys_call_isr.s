bits 32
global sys_call_isr
extern sys_call			; The C function that sys_call_isr will call

;;; System call interrupt handler. To be implemented in Module R3.

section .text
sys_call_isr:
	push EBP  ;pushing all registers onto stack
	push EDI
	push ESI
	push EDX
	push ECX
	push EBX
	push EAX
	push SS
	push ES
	push DS
	push FS
	push GS
	
	push ESP
	call sys_call
	mov EAX, ESP

	pop GS
	pop FS
	pop DS
	pop ES
	pop SS
	pop EAX
	pop EBX
	pop ECX
	pop EDX
	pop ESI
	pop EDI
	pop EBP
	iret
