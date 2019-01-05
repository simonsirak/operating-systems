bits 32	
	
section .multiboot_header

magic equ 0xe85250d6	    		; multiboot 2
arch  equ 0		    		; protected mode i386
	
header_start:
    dd magic                		; magic number 
    dd arch                 		; architecture
    dd header_end - header_start 	; header length
    ; checksum
    dd 0x100000000 - (magic + arch + (header_end - header_start))

    ; insert optional multiboot tags here

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:
