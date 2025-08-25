#include "./pit.h"

void pit_init(uint32_t freq)
{
	outb(PIT_CHANNEL,0x36);	//setvalue
	uint16_t val=PIT_FREQ/freq; //calculate value
	
	outb(PIT_VAL_PORT,val&0xff);	//send lower byte
	
	outb(PIT_VAL_PORT,(val>>8)&0xff);	//send upper byte
	
}
