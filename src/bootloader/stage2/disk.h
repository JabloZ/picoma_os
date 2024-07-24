#pragma once
#include <stdint.h>
#include <stdbool.h>
typedef struct{
    uint8_t id;
    uint16_t cylinders;
    uint16_t sectors;
    uint16_t heads;
} DISK;
bool disk_initialize(DISK* disk, uint8_t drive);
bool disk_read_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void* data);