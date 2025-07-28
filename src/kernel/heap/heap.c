#include "heap.h"

uint8_t* heap_block_table=(uint8_t*)0x2000000;
void heap_init()
{
	for(int i=0;i<HEAP_COUNT;i++)
		heap_block_table[i]= 0x00;
		
	}
	
void free(void* ptr)
{
	
	}
