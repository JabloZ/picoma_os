export PATH=$PATH:/usr/local/i386elfgcc/bin

nasm "bootloader/boot.asm" -f bin -o "binary/boot.bin"
nasm "bootloader/kernel_entry.asm" -f elf -o "binary/kernel_entry.o"
i386-elf-gcc -ffreestanding -m32 -g -c "kernel/kernel.c" -o "binary/kernel.o"
nasm "bootloader/zeroes.asm" -f bin -o "binary/zeroes.bin"

i386-elf-ld -o "binary/m_kernel.bin" -Ttext 0x1000 "binary/kernel_entry.o" "binary/kernel.o" --oformat binary

cat "binary/boot.bin" "binary/m_kernel.bin" "binary/zeroes.bin"  > "binary/OS.bin"

qemu-system-x86_64 -drive format=raw,file="binary/OS.bin",index=0,if=floppy,  -m 128M