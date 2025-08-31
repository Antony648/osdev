#include "./partitions.h"
#inlcude "../heap/heap_cream.h"
//create karray
uintptr_t karray[5]={0,0,0,0,0};		//so that we can use single disk resolution independently
extern struct disk* motherlobe[5];
struct partition* single_check_part(struct disk* disk_1,void* part_entry)
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
	
	j->fs_type=val[0x04];
		
	uint32_t* val_32=NULL;
	val_32=(uint32_t*)val[0x08];
	
	j->start_sect=val[0];
	j->sect_num=val[1];
	j->f_disk=disk_1;
	
	heap_cream_free(karray,(void*)val);
	return j;
}
struct partition* check_part(struct disk* disk_1, void* part_table)
{
	void* vale=NULL;
	uint8_t* helper=(uint8_t*)part_table;
	struct partition* head=single_check_part(disk_1,part_table);	//this will fill the head with the first entry we 
	//are certian all disks have at least a parition
	if(!head)
		return NULL;		//blank disk no partitions
	
	struct partition* last= NULL;
	head=last;
	for(int i=1;i<4;i++)
	{
		vale=single_check_part(disk_1,(void*)helper[i*16]);
		if(!vale)
			continue;
		last->next=vale;
		last=vale;
		
	}
	return head;
}
void disk_partition_scanner(struct disk* disk_1)
{
	
	
	//create a linkelist start pointer set ti to NULL
	void* buffer= heap_cream_malloc(karray,512);
	//create buffer
	read_disk_block(disk_1,0, 1, buffer);
	//use disk to functions to load data in buffer
	uint8_t* k=(uint_8*)buffer;
	
	disk_1.link_list=check_part(disk_1,(void*)k[446]);
	// read data as 16 byte chunks from byte 446 to 510
	//analyse 16 bytes if it is a recognised fs then create add it to linked list
	//repeat process for all four chunks and return linked list start address
	heap_cream_free(karray,buffer);
}
void scan_part_all_disks()
{
	heap_cream_init(karray);
	for(int i=0;i<5;i++)
	{
		if(motherlobe[i]!=NULL)
			disk_partition_scanner(motherlobe[i]);
	}
}

