CREATE DISK SCANNER

WORKDONE:
	replaced dummy functions with actual functions that can scan for primary master, secondary master, primary slave, secondary
	master and secondary slave, this is dected by by 0x80, 0x81,0x82 and 0x83 , and creates an entry if disk found, and fills some
	attributes of the disk, no testing till now...

NOTES:
	-[notes on current disk.c](../notes/disk_scanner_c)
	-[notes on current disk.h](../notes/disk_scanner_h)

TOOLS:
	vim, qemu,geany
