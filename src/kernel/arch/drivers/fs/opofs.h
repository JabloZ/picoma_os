
#pragma once
#include "../../disk.h"
#include "../fdc.h"
#include "../../../clibs/stdint.h"
#include "../../../clibs/stdio.h"
#include "../../../clibs/string.h"
#include "../../../clibs/stdbool.h"
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
char global_prefix[100];
file_entry root_dir;
void init_opofs(uint32_t disk);
bool find_file_opo(uint32_t disk, char* path, file_entry* file_e, file_entry* file_test, file_entry *end_file);
int read_file_opo(uint32_t disk, file_entry* fe, uint8_t* buf);