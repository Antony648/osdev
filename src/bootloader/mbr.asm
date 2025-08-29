ORG		0x7c00
[BITS 16]
cylinder_no: dw 0x00
sect_no:	db 0x00
head_no:	db 0x00

start:
	mov 	ax,0x00
	mov		ss,ax
	mov		ds,ax
	mov		es,ax
	mov		sp,0x6000	;we are going to put stack before 0x6000
	;so 0x6000 to 0x7c00 doesnot contian any data
	jmp		chs_end
chs:
;lba at bx
	mov		ah, 0x08
	mov		dl, 0x80
	int		0x13
	jc		.error
	mov		al,dh
	inc 	al
	and		cl,0x3f	;sector in cl
	mul		cl
	mov		dx,bx
	mov		bx,ax
	mov		ax,dx
	div		bx
	mov		bx,ax	;bx contains cylinder
	mov		ax,dx	;ax contains temp
	div		cl		
	;head in al
	inc		ah
	;sector in ah
	mov		cl,ah
	and		cl,0x3f
	mov		dh,al
	mov		ch,bl
	shl		bh,6
	or		cl,bh
	;movzx	ax,cl
	;and		ax,0xc0
	;shl		ax,2
	;mov		al,ch
	;mov		cylinder_no,ax
	
		
	
	ret
.error:
	jmp		$
	ret
chs_end:
;code for copying mbr to 0x600

;end of copying mbr to 0x600
jmp		$-0x1c00
;code for copying vbr to 0x7c00
;end of code for copying to 0x7c00
jmp		0x000:0x7c00
times 446-($-$$) db 0x00
;partition table begin

;partition table end
dw 0x55aa
