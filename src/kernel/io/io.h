#ifndef IO_H
#define IO_H
unsigned char inb(unsigned short port_no);
unsigned short in16(unsigned short port_no);
unsigned int in32(unsigned short port_no);

void outb(unsigned short port_no,unsigned char data);
void out16(unsigned short port_no,unsigned short data);
void out32(unsigned short port_no,unsigned int data);
#endif
