all:
	nasm ./src/bootloader/boot.asm -f bin -o ./build/bootloader/boot.bin

