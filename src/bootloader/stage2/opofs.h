
#pragma once
#include "stdint.h"
#include "disk.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#define HEAD 2
#define SECTOR 18
#define CYLINDER 80
typedef struct
{
    uint8_t filename[15];
    uint8_t is_dir;
    uint32_t size;
    uint32_t lba_first;
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
bool find_file_opo(DISK* disk, char* path, file_entry* file_e, file_entry* file_test);