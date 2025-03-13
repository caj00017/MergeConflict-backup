bits 32
global sys_call_isr
extern sys_call			; The C function that sys_call_isr will call

;;; System call interrupt handler. To be implemented in Module R3.

 sys_call_isr:
	; mov EAX, -1
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

	call sys_call
	mov ESP, EAX

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
