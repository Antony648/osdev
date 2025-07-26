#ifndef	 IDT_H
#define  IDT_H
#include <stdint.h>

#define  32INTERRUPT_KC_3_1 0xee
#define  32TRAP_KC_3_1		0xef

#define 32INTERRUPT_KC_2_1  0xce
#define 32TRAP_KC_2_1		0xcf

#define 32INTERRUPT_KC_1_1 	0xae
#define 32TRAP_KC_1_1       0xaf

#define 32INTERRUPT_KC_0_1	0x8e
#define 32TRAP_KC_0_1		0x8f
struct idt_desc
{
	uint16_t offset_l;	//offset bits 0-15
	uint16_t selector;	//selector of LDT or GDT
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
