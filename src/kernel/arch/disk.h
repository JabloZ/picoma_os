#ifndef DISK_H
#define DISK_H
#include "../clibs/stdint.h"
void __attribute__((cdecl)) ata_lba_read(uint32_t lba, uint32_t sectors, void* data_out);
#endif