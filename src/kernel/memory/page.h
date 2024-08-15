#include "../clibs/stdint.h"
#define PAGE_SIZE 4096
#define PAGE_TABLE_COUNT 1024
#define PAGE_DIR_COUNT 1024
typedef struct{
    uint32_t present: 1;
    uint32_t read_write: 1;
    uint32_t user: 1;
    uint32_t pwt: 1;
    uint32_t pcd: 1;
    uint32_t accessed: 1;
    uint32_t reserved_1: 1;
    uint32_t page_size: 1;
    uint32_t reserved_2: 4;
    uint32_t frame_addr: 20;
} __attribute__((packed)) page_directory_entry;

typedef struct{
    uint32_t present: 1;
    uint32_t read_write: 1;
    uint32_t user: 1;
    uint32_t pwt: 1;
    uint32_t pcd: 1;
    uint32_t accessed: 1;
    uint32_t dirty: 1;
    uint32_t pat: 1;
    uint32_t global: 1;
    uint32_t reserved: 3;
    uint32_t frame_addr: 20;
} __attribute__((packed)) page_table_entry;

void enable_paging(page_directory_entry *pg);
void init_page();

typedef struct{
    page_table_entry pages[1024] __attribute__((aligned(4096)));
} page_table_t;

typedef struct{
    page_table_t* page_tables[1024];
    uint32_t* first_table;
    uint32_t cr0;
} page_directory_t;

page_directory_t g_page_dir;