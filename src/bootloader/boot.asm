org		0x7c00
bits	16

jmp		short	start 
nop
OEM_idenifier:		db	'MSWIN4.1'
bytes_per_sect:		dw	512
sect_per_clust:		db	1
no_res_sect:		dw	1
fat_no:		 	db 	2
root_dir_entries:	dw	0xe0
no_sectors:		dw	2880
media_desc:		db	0x0f0	
sect_per_fat:		dw 	9
sect_per_track:		dw 	18
heads:			dw 	2
hiddensect:		dd	0
large_sect_no:		dd 	0
;extended boot record
drive_no:		db	0
flag:			db 	0
signature:		db 	0x29
volume_id:		db	0x12,0x34,0x56,0x78
volume_label:		db	'antony   os'
identifier_str:		db 	'fat12   '
start:
	jmp 	main
nop
;expects name of file to be read in si
;INT 0x13, AH = 2 -- read floppy/hard disk in CHS mode
;AH = 0x02 → Function number (Read sectors)
;AL = Number of sectors to read (must be between 1-127)
;CH = Cylinder number (tracks, 0-based)
;CL = Sector number (1-63) → Bits:
;   Bits 0-5 → Sector number (1-63, not 0-based)
;   Bits 6-7 → High bits of Cylinder number (if needed)
;DH = Head number (0-1 for a floppy disk)
;DL = Drive number (0 for first floppy, 1 for second)
;ES:BX = Buffer address → Where data should be stored

;expects 
read_sector:
	push	ax	
	push	dx

	mov		ah,0x02
	mov		al,1
	mov 	dl,[drive_no]
	pusha
	INT		0x13
	popa

	pop		dx
	pop 	ax
	ret

setup_:
;set all register in condition expected by the system
;jmp control to region in ram that stores the buffer of kernel.bin
;so that cpu exectues it
	xor ax,ax
	ret
kern:	db		"KERNEL   BIN"
start_boot:
;reseverd sector number =1
;fat region=no_fat*sect_per_fat
;root_reg_sect=reserv_sect+fat_region
;we copy directory entries into buffer(or direxcly access from img)
;and compare the name with "KERNEL  BIN" bad practice but we will fix later
;then copy sectors from kenel.bin file to buffer in ram
;call setup_

;region for calculation of byte number associated with the root sector
;	mov 	cx,[fat_no]		;number of fat tables
;	mul	cx,[sect_per_fat]	;contains the fat region in sectors-1 because we forgot to add the boot sector
;	add	cx,1			;contains the fat region in sectors
;	mul	cx,[bytes_per_sect]	;contains the byte address
;

.loop:
	
	;code for 11byte string comparision
	lea 	si,[kern]	;now si contains the value of "KERNEL   BIN"
;	lea	di,[cmp_str]
	mov 	cx,11		;number of bytes to be compared stored in cx

	cld			;clear direction flag
	repe 	cmpsb		;compare all bytes
	jz 	.match

.match:
	ret



;expects string data in si
;INT 0x10, AH = 0xE,AL=char val,bh=0x0 for printing in BIOS
print:
	push  	ax	;ax=ah,al
	push	bx
push 	si
	mov		al,[si]
.loop:
	test 	al,al
	jz		.end
	mov		ah,0x0e
	mov 	bh,0x00
	INT		0x10
	lodsb
	jmp		.loop
	.end:
	pop		si
	pop 	bx
	pop		ax
	ret
msg:	db 'success',0
main:
	;set all segment registers to 0
	;we cannot direcly push 
	xor 	ax,ax
	mov		ss,ax
	mov		ds,ax
	mov 	es,ax
	mov 	sp,0x7c00

	mov 	si,msg
	call	print
	hlt
.halt:
	jmp 	.halt

times 510-($-$$) db 0
dw 	0xaa55
