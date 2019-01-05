bits 16

	;; When this master boot record is loaded by the BIOS it will be
        ;; loaded to adress 0x07c00. Segment registers are shifted
	;; 4-bits before OR:ed with the offset. 
start:
	mov ax, 0x07C0		; 0x07c00 is were we are
	add ax, 0x20		; add 0x20 (when shifted 512)
	mov ss, ax              ; set the stack segment 
	mov sp, 0x1000          ; set the stack pointer (+ 4096)

	mov ax, 0x07C0		; Set data segment to where we're loaded
	mov ds, ax              ; you will learn more about this later

	mov si, msg		; move a pointer to the message into SI register

	mov ah, 0x0E		; 0x0e is the id of the print char BIOS procedure

.next:
	lodsb			; get one byte from SI and place in AL
	cmp al, 0               ; if the byte is zero
	je .done		;    jump do done
	int 0x10		; otherwise, invoke the BIOS system call 
	jmp .next               ; next byte 
	
.done:
	jmp $			; loop forever


msg:	db 'Hello', 0 		; the string we want to print (terminated by zero)

	times 510-($-$$) db 0	; zeros to fill up the 512 bytes
	dw 0xAA55		; master boot record signature 
	
