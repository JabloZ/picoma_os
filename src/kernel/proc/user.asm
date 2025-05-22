global enter_usermode
extern test_user_function
enter_usermode:
    mov ax, (4 * 8) | 3
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax

	; set up the stack frame iret expects
	mov eax, esp
	push (4 * 8) | 3 
	push eax 
	pushf
	push (3 * 8) | 3 
	push test_user_function 
	iret
global test2
test2:
	mov eax, 0
	mov ebx, 0x100000
	int 0x80