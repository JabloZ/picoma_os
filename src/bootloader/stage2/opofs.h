
#include "stdint.h"
#include "disk.h"
#include "stdio.h"
#define HEAD 2
#define SECTOR 18
#define CYLINDER 80
typedef struct
{
    uint8_t filename[15];
    uint32_t size;
    uint32_t lba_first;
    uint8_t is_dir;
    uint8_t reserved[8];
} __attribute__((packed)) file_entry;

typedef struct{
    file_entry file_data;
    uint8_t file_mem[512];
} file_entry_data;

typedef struct{
    uint32_t lba;
    uint8_t mem[512];
} __attribute__((packed)) boot_sector;

void init_opofs(DISK* disk);