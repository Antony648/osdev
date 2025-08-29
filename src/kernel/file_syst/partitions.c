#include "./partitions.h"
#inlcude "../heap/heap_cream.h"
//create karray
uintptr_t karray[5];
struct partition* disk_partition_scanner(struct disk* disk_1)
{
	heap_cream_init(karray);
	struct partition *head;
	//create a linkelist start pointer set ti to NULL
	
	//create buffer
	//use disk to functions to load data in buffer
	// read data as 16 byte chunks from byte 446 to 510
	//analyse 16 bytes if it is a recognised fs then create add it to linked list
	//repeat process for all four chunks and return linked list start address
}
