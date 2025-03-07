bits 32
global sys_call_isr

;;; System call interrupt handler. To be implemented in Module R3.

extern sys_call			; The C function that sys_call_isr will call
sys_call_isr:
	mov eax, -1		;; DELETE THIS LINE WHEN YOU IMPLEMENT R3
	iret

; extern sys_call			; The C function that sys_call_isr will call
; sys_call_isr:
; 	push ESP
; 	push EBP
; 	push EDI
; 	push EDX
; 	push ECX
; 	push EBX
; 	push EAX
; 	push SS
; 	push GS
;	push FS	
;	push ES
;	push DS
;	call sys_call
;	mov ESP, EAX
;	pop DS
;	pop ES
;	pop FS
;	pop GS
;	pop SS
;	pop EAX
;	pop EBX
;	pop ECX
;	pop EDX
;	pop ESI
;	pop EDI
;	pop EBP
;	pop ESP
;	iret
