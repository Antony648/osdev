#include "osconfig.h"
#include "idt.h"
#include "essenitals.h"
struct idt_desc idt_table[TOTAL_INTERRUPTS];
struct idtr idtr_descriptor;

void idt_init()
{
    memset(idt_table,0,sizeof(idt_table));
    idtr_descriptor.len=sizeof(idt_table)-1;
    idtr_descriptor.addr=idt_table;
}

