section .asm
global inb
global in16
global in32

global outb
global out16
global out32
inb:
	push	ebp
	mov	ebp,esp
	mov	dx,[ebp+8]	;first param on dx
	xor	al,al
	in	al,dx

	pop	ebp
	ret

in16:
	push	ebp
	mov	ebp,esp
	xor	ax,ax
	mov	dx,[ebp+8]
	in	ax,dx
	pop	ebp
	ret

in32:	
	push	ebp
	mov	ebp,esp
	xor	eax,eax
	mov	dx,[ebp+8]
	in	eax,dx
	pop	ebp
	ret

outb:
	push 	ebp
	mov	ebp,esp
	mov	al,[ebp+12]	;second param in al, 8bit val
	mov	dx,[ebp+8]
	out	dx,al
	pop	ebp
	ret

out16:
	push	ebp
	mov	ebp,esp
	mov	ax,[ebp+12]	;second param in al, 16bit val
	mov	dx,[ebp+8]
	out	dx,ax
	pop	ebp
	ret
out32:
	push	ebp
	mov	ebp,esp
	mov	eax,[ebp+12]
	mov	dx,[ebp+8]
	out	dx,eax
	pop	ebp
	ret
