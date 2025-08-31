#include "./partitions.h"
#inlcude "../heap/heap_cream.h"
//create karray
uintptr_t karray[5];
//extern motherlobe;
struct partition* single_check_part(void* part_entry)
{
	uint8_t* val=(uint8_t*)heap_cream_malloc(karray,16);
	val=(uint_8*)part_entry;
	
	if(val[4] == 0x00)
	{
		heap_cream_free(karray,(void*)val);
		return NULL;
	}
	struct partition* j=heap_cream_malloc(karray,sizeof(struct partition));
	j->next=NULL;
	if(val[0]==0x80)
		j->is_bootable=1;
	else
		j->is_bootable=0;
	
	
	heap_cream_free(karray,(void*)val);
	return j;
}
struct partition* check_part( void* part_table)
{
	void* vale=NULL;
	uint8_t* helper=(uint8_t*)part_table;
	struct partition* head=single_check_part(part_table);	//this will fill the head with the first entry we 
	//are certian all disks have at least a parition
	if(!head)
		return NULL;		//blank disk no partitions
	
	struct partition* last= NULL;
	head=last;
	for(int i=1;i<4;i++)
	{
		vale=single_check_part((void*)helper[i*16]);
		if(!vale)
			continue;
		last->next=vale;
		last=vale;
		
	}
	return head;
}
struct partition* disk_partition_scanner(struct disk* disk_1)
{
	heap_cream_init(karray);
	
	//create a linkelist start pointer set ti to NULL
	void* buffer= heap_cream_malloc(karray,512);
	//create buffer
	read_disk_block(disk_1,0, 1, buffer);
	//use disk to functions to load data in buffer
	uint8_t* k=(uint_8*)buffer;
	check_part((void*)k[446]);
	// read data as 16 byte chunks from byte 446 to 510
	//analyse 16 bytes if it is a recognised fs then create add it to linked list
	//repeat process for all four chunks and return linked list start address
	heap_cream_free(karray,buffer);
}

