#!/bin/sh

i386-elf-as boot.s -o boot.o
i386-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -T link.ld -o gOS.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
if grub-file --is-x86-multiboot gOS.bin; then
  mkdir -p isodir/boot/grub
  cp gOS.bin isodir/boot/gOS.bin
  cp grub.cfg isodir/boot/grub/grub.cfg
  grub-mkrescue -o gOS.iso isodir
  rm -rf isodir
else
  echo binary is not valid multiboot
fi
rm *.o
