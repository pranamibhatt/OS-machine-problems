#!/bin/sh

sudo mount -o loop dev_kernel_grub.img /mnt/floppy/
sudo cp kernel.bin /mnt/floppy/
sleep 2
sudo umount /mnt/floppy/
bochs -f bochsrc.bxrc 


