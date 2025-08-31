rm disk.img
nasm -f bin -g ../mbr.asm -o ./mbr.bin
dd if=mbr.bin of=disk.img conv=notrunc
dd if=fboot.bin of=disk.img bs=512 seek=1 conv=notrunc
qemu-system-i386 -hda ./disk.img
