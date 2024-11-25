#ifndef DISK_H
#define DISK_H
#include "stdint.h"
#include "stdbool.h"
#define HEAD 2
#define SECTOR 18
#define CYLINDER 80
typedef struct{
    uint8_t id;
    uint16_t cylinders;
    uint16_t sectors;
    uint16_t heads;
} DISK;
bool disk_initialize(DISK* disk, uint8_t drive);
bool disk_read_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void* data_out);
#endif