#pragma once

//500 start from bootloader
#define MEMORY_MIN 0x00000500
#define MEMORY_MAX 0x00080000

//fat and mem attribs - load fat at 0x20000, load 64kib
#define FAT_ADDR 0x20000
#define FAT_SIZE 0x00010000


//elf
#define ELF_LOAD_ADDR 0x30000
#define ELF_LOAD_SIZE 0x10000

//kernel attribs - load
//0x40000 start, and load 1mb

#define MEMORY_LOAD_KERNEL 0x50000
#define MEMORY_SIZE 0x10000
#define KERNEL_LOAD_ADDR 0x100000
