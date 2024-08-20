#include "../clibs/stdint.h"
#include "../../glibs/memory_params.h"
#include "vmm.h"
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
page_table_entry* create_page_table();
void map_page(void* phys_addr, void* virt_addr) ;



page_directory_entry page_directory[PAGE_DIR_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry first_page_table[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));\

