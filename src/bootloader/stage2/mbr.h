#ifndef MBR_H
#define MBR_H

#include "stdint.h"
#include "disk.h"

typedef struct{
    uint8_t drive_atr;
    uint8_t chs_start[3];
    uint8_t type;
    uint8_t chs_end[3];
    uint32_t lba_start;
    uint32_t sectors_num;
} __attribute__((packed)) Mbr_entry;

typedef struct{
    DISK* disk;
    uint32_t partition_offset;
    uint32_t partition_size;
} Partition;
#endif