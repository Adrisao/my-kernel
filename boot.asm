section .multiboot
align 4
	dd 0x1BADB002
	dd 0x00
	dd -(0x1BADB002)

section .text
global _start
extern kernel_main

_start:
	call kernel_main
	cli
.hang:
	hlt
	jmp .hang
