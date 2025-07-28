#ifndef HEAP_H
#define HEAP_H
#include "../osconfig.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_START 		0x100000
#define start_block  	0xc1
#define end_block		0x01
#define middle_block	0x81
#define single_block	0x41
#define unallocated 	0x00
#define HEAP_COUNT		25600  //no of 4kb pages possible in 100 mb 
#endif
