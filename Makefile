ASM=nasm
BUILD_DIR=build
SRC_DIR=src
CC=gcc

.PHONY:	all bootloader kernel floppy_img clean 	always	tools

floppy_img:$(BUILD_DIR)/floppy_bootable.img
$(BUILD_DIR)/floppy_bootable.img:kernel bootloader tools
	dd if=/dev/zero	of=$(BUILD_DIR)/floppy_bootable.img bs=512 count=2880
	mkfs.fat -F 12 -n 'NBOS'  $(BUILD_DIR)/floppy_bootable.img
	dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/floppy_bootable.img conv=notrunc
	mcopy -i $(BUILD_DIR)/floppy_bootable.img	$(BUILD_DIR)/kernel.bin "::kernel.bin"
	mcopy -i $(BUILD_DIR)/floppy_bootable.img	text.txt "::text.txt"

kernel:$(BUILD_DIR)/kernel.bin
$(BUILD_DIR)/kernel.bin:always
	$(ASM)	$(SRC_DIR)/kernel/kernel.asm -f bin -o $(BUILD_DIR)/kernel.bin

bootloader:$(BUILD_DIR)/boot.bin
$(BUILD_DIR)/boot.bin:always
	$(ASM)	$(SRC_DIR)/bootloader/boot.asm -f bin -o $(BUILD_DIR)/boot.bin

tools:$(BUILD_DIR)/tools.out
$(BUILD_DIR)/tools.out:always
	$(CC)	$(SRC_DIR)/tools/fat.c -o $(BUILD_DIR)/tools.out

always:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf	$(BUILD_DIR)/*
