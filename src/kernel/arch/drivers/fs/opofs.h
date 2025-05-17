
#ifndef OPOFS_H
#define OPOFS_H
#include "../../disk.h"
#include "../fdc.h"
#include "../../../clibs/stdint.h"
#include "../../../clibs/stdio.h"
#include "../../../clibs/string.h"
#include "../../../clibs/stdbool.h"
#include "../../../memory/kalloc.h"

#define HEAD 2
#define SECTOR 18
#define CYLINDER 80
void b();
typedef struct file_entry{
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
bool find_file_opo(uint32_t disk, char* path, file_entry* file_e, file_entry* file_test, file_entry *end_file, int* lba_of_filedir);
int read_file_opo(uint32_t disk, file_entry* fe, uint8_t* buf);
int change_file_entry_opo(file_entry* file_e, int new_size, int new_lba);
int create_file_opofs(file_entry* og_dir, int lba_where_to_create,  char* new_filename, int new_lba, int new_size, uint8_t* new_file_data, int dir);
int free_find_space_for_file(int size_to_fit);
void opo_path_formatter(char* path, char* path_new, int count_elements);
int setup_global_file_info_table();
int find_free_sectors_in_disk(int size);
int getbit(uint8_t* bits, int bit);
int setbit(uint8_t* bits, int bit, int val);
int allocate_sectors(int lba_start, int sectors);
int delete_file_or_dir(file_entry* file_to_delete, int parent_lba);

int execute_or_recognize_command();
int move_line_down();
int recognize_command(char* command);
int return_file_entry_with_name(char* name, file_entry* save_f, file_entry* file_e);
int return_file_entry(int i, file_entry* save_f, file_entry* file_e);
void left_arrow_func();
void right_arrow_func();
void down_arrow();
void up_arrow();
int move_to_left(int cur);
int only_null_until_end_of_file(int x, int y);
int only_null_until_end(int x, int y);
void quit_pte_func();
#endif