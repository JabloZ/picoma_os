#ifndef PAGE_H
#define PAGE_H
#include "../clibs/stdint.h"
#include "../clibs/memory.h"
#include "../clibs/stdio.h"
#include "../memory_params.h"
#include "kalloc.h"
#include "pmm.h"
#include "../../kernel/arch/gdt/gdt.h"
#include "../../kernel/arch/interrupts/idt.h"
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


void init_page();
void *vmm_alloc_page_4kb(void *virtual_address);
void vmm_map_page_4mb(uint32_t virtual_address, uint32_t physical_address);
void vmm_map_page_4kb(uint32_t virtual_address, uint32_t physical_address);
void vmm_unmap_page_4kb(void* virtual_address);
page_directory_entry page_directory[PAGE_DIR_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry page_table[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
void __attribute((cdecl)) reload_segments();
void vmm_memory_status();
#endif