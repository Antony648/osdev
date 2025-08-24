# include "isr.h"
#include "../kernel.h"
#include "../io/io.h"
volatile uint64_t tick=0;
void divide_zero()
{
	print("divide by zero error");
	}
void int21_handler()
{
	print("test int 0x21");
	//char a=inb(0x60);
	//print(&a);
	}
void int20_handler()
{
	//print("int 20");
	tick++;
	//print("int20\n");
}
void no_interrupt()
{
	//print("isr not written yet..hehe");
	}
