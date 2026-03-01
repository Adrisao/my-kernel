#include <stdlib.h>

int main(){
	system("rm -f *.o kernel.bin myos.iso");
	system("rm -rf isodir");
	system("nasm -f elf32 boot.asm -o boot.o");
 	system("gcc -m32 -ffreestanding -c kernel.c -o kernel.o");
	system("ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o");
 	system("file kernel.bin");
	system("mkdir -p isodir/boot/grub");
	system("cp kernel.bin isodir/boot/");
	system("cp boot/grub/grub.cfg isodir/boot/grub/");
	system("grub-mkrescue -o myos.iso isodir");
	return 0;
}
