#ifndef PARTITIONS_H
#define PARTITIONS_H
#include "../disk/disk.h"
#include <stdint.h>
struct partition
{
	struct disk* f_disk;
	enum FILE_SYST_TYPE fs_type;
	uint32_t start_sect;
	uint32_t sect_num;
	uint8_t is_bootable;
	struct partition* next;
};
struct partition* disk_partition_scanner(struct disk*);
void scan_part_all_disks();
#endif
