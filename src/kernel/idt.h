#ifndef	 IDT_H
#define  IDT_H

struct idt_desc
{
	uint16_t offset_l;	//offset bits 0-15
	uint16_t selector;	//selector of LDT
	uint8_t zero=0x00;		//set to zero
	uint8_t attr;		//gate type
	uint16_t offset_h; //offset bits 16-31
}__attribute__((packed));

struct idtr
{
	uint16_t len;	//size of idt-1 in bytes 
	uint32_t addr;  //address of idt
}__attribute__((packed)); 
#endif
