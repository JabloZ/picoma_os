#pragma once
#include "../../clibs/stdint.h"
typedef struct{
    uint8_t boot;
    uint8_t head_start;
    uint8_t sector_start;
    uint8_t cylinder_start;
    uint8_t system_id;
    uint8_t head_end;
    uint8_t sector_end;
    uint8_t cylinder_end;
    uint32_t lba_sector;
    uint32_t size;
} __attribute__((packed)) partition_table;

typedef struct{
    uint8_t mbr_bootstrap[446];
    partition_table partitions[4];
    uint16_t signature;
} __attribute__((packed)) mbr_t;

typedef struct{
    uint32_t partition_offset;
    uint32_t partition_size;
} partition;