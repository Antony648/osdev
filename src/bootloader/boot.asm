org	0x7c00
bits	16

jmp	short	start
nop

bdb_oem:		db	'MSWIN4.1'
bdb_bytes_per_sector:	dw		512
bdb_sector_per_cluster:	db	1
bdb_reserved_sector:	dw	1
bdb_fat_count:		db	2
bdb_root_number:	dw	0x0e0
bdb_number_sectors:	dw	2880
bdb_media_desc:		db	0x0f0
bdb_sector_per_fat:	dw	9
bdb_sector_per_track:	dw	18
bdb_heads:		dw	2
bdb_hidden_sect:	dd	0
bdb_large_sect_count:	dd	0
;extended boot record
ebr_drive_num:		db	0
ebr_flag:		db	0
ebr_signature:		db	0x29
ebr_volume_id:		db	0x12,0x34,0x56,0x78
ebr_volume_label:	db	'antony   os'
ebr_file_sys_indentify:	db	'fat12   '
start:
	jmp	main
;the the lba value is passed to ax
;expected after call
;dh =head
;cx[0-5]=sector
;cx[6-15]=cylinder
lba_to_chs:
	push	ax
	push 	dx
	
	xor 	dx,dx
	div	word [bdb_sector_per_track]
	
	;dx has (lba% sector_per_track)
	;ax has(lba/sector_per_track)
	inc 	dx	;
	mov	cl,dl
	;now cl contains the sector number and we assume that the 2 msb are 0

	div	word	[bdb_heads]
	
	;ax has(lba/sector_per_track)/head
	;dx has(lba/sector_per_track)%head

	mov 	dh,dl 	;copying the value in lower bit
	mov 	ch,al	
	shl	ah,6
	or	cl,ah

	pop 	ax
	mov	dl,al
	pop	ax
	ret
;read_from_disk
;ah=02
;al=no_of_sectors
;ch=track/cylinder
;cl=sector
;dh=head
;dl=drive_no
;es:bx
;lba ax for lba_to_chs
;set cx as [no_of_sectors] we will store it in ah later after lba_to_chs
;we expect cx to store no_of_sectors, bx to point to read location in device,dl driver no
read_disk:
	push 	ax
	push	bx
	push 	cx
	push	dx

	push	cx		;this is for setting ah to [no_of_sectors] later
	call 	lba_to_chs	;after this dh,cl,ch
	pop	ax		;al has [no_of_sectors]
	mov	ah,0x02		;ah has 0x02
	mov	di,3
.loop:
	int	0x13
	jc	.handle_err
	jmp	.done
.fail_read: 
	push	si
	mov 	si,read_fail
	call	puts
	pop	si
	jmp	.exit_read
.done:
	push 	si
	mov	si,read_success
	call	puts
	pop	si
.exit_read:
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	ret
.handle_err:
	dec	di

	push	si
	mov 	si,read_try
	call	puts
	pop	si

	cmp	di,0
	je	.fail_read
	jmp	.loop
		
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
%define	ENDL	0x0a,0x0d
msg:		db 	"helloantos",ENDL,0
read_fail:	db  	"fail",0
read_try:	db 	"try",0
read_success:	db	"success",ENDL,0

main:
	xor 	ax,ax
	mov 	ss,ax
	mov	ds,ax
	mov	es,ax
	mov 	sp,0x7c00
	
	mov	si,msg
	call	puts
	
	mov 	[ebr_drive_num],dl
	mov	bx,0x7e00
	mov	cx,0x01
	
;we expect cx to store no_of_sectors, bx to point to read location in device,dl driver no
	call 	read_disk	
	hlt
.halt:
	jmp	.halt

times 510-($-$$) db 0
dw 0xaa55

