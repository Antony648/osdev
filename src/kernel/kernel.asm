org	0x7c00
bits	16

start:
	jmp	main
puts:
	push 	si
	push 	ax
.loop:
	lodsb	;increments si and loads value at si to al
	test 	al,al
	jz	.done
	;parameters for bios char print
	;ah=0x0e
	;bh=0x00

	mov 	ah,0x0e
	mov	bh,0x00
	int	0x10
	jmp 	.loop
.done:
	pop 	ax
	pop 	si
	ret

msg:	db 'hello form antosbootloader',0

main:
	mov 	ax,0
	mov 	ss,ax
	mov	ds,ax
	mov	es,ax
	mov 	sp,0x7c00
	
	mov	si,msg
	call	puts
	hlt
.halt:
	jmp	.halt

times 510-($-$$) db 0
dw 0xaa55
	
