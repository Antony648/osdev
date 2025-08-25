CREATED HEAP32 A FIRST STEP TO A SLAB IMPLEMENTATION

WORKDONE:
	created an interface for breaking an allocated space of 4kb into mangable chunks of 32bytes, created a small system that keeps
	in track of what blocks make up a single chunk, we can have aboout 124 of such 32 byte blocks, and upper 4blocks for bookeeing
	upper 4 blocksin the sense 128 bytes,124 bytes of each block info last 4 bytes to indicate the end
	
NOTES:
	-[full notes on heap_cream.h](../notes/heap_cream_h)
	-[full notes on heap_cream.c](../notes/heep_cream_c)
	
	
TOOLS:
	vim,geany,qemu,gdb
