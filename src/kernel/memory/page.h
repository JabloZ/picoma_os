#ifndef PAGE_H
#define PAGE_H
#include "../clibs/stdint.h"
#include "../clibs/memory.h"
#include "../clibs/stdio.h"
#include "../memory_params.h"

#include "pmm.h"
#include "../../kernel/arch/gdt/gdt.h"
#include "../../kernel/arch/interrupts/idt.h"
#define PAGE_SIZE 4096
#define PAGE_TABLE_COUNT 1024
#define PAGE_DIR_COUNT 1024
#define KERNEL_SPACE 0xC0000000
#define VIRT_TO_PHYS(addr) ((uint32_t)(addr) - KERNEL_SPACE)

typedef uint32_t page_directory_entry;
typedef uint32_t page_table_entry;
enum page_table_flags{
    PAGE_PRESENT=1,
    PAGE_WRITABLE=2,
    PAGE_USER=4,
    PAGE_WRITE_THOUGH=8,
    PAGE_NOT_CACHE=0x10,
    PAGE_ACCESSED=0x20,
    PAGE_DIRTY=0x40,
    PAGE_PAT=0X80,
    PAGE_GLOBAL=0x100,
    PAGE_LV4=0x200,
    PAGE_FRAME_ADDR=0xFFFFF000 //JAK COS NAGLE PRZESTANIE DZIALA SPROBUJ ZMIENIC NA 0X7FFFF
};
enum page_dir_flags{
    PAGE_DIR_PRESENT=1,
    PAGE_DIR_WRITABLE=2,
    PAGE_DIR_USER=4,
    PAGE_DIR_WRITE_THOUGH=8,
    PAGE_DIR_NOT_CACHE=0x10,
    PAGE_DIR_ACCESSED=0x20,
    PAGE_DIR_DIRTY=0x40,
    PAGE_DIR_PAT=0X80,
    PAGE_DIR_GLOBAL=0x100,
    PAGE_DIR_LV4=0x200,
    PAGE_DIR_FRAME_ADDR=0xFFFFF000
};

page_table_entry kernel_zero[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry kernel_first[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry kernel_second[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));

void reset_stack();
void flush_tlb_single(uint32_t addr); 
void init_page();
//void *vmm_alloc_page_4kb(void *virtual_address);
void vmm_alloc_page_4mb(page_table_entry* page_tab,uint32_t virtual_address, uint32_t phys_addr);
void* vmm_alloc_page_4kb(page_table_entry* page_tab,uint32_t virtual_address, uint32_t physical_address);
void vmm_unmap_page_4kb(page_table_entry* page_tab, uint32_t virtual_adr);
page_directory_entry page_directory[PAGE_DIR_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry page_table[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
void __attribute((cdecl)) reload_segments();
void vmm_memory_status();
void switch_current_dir(uint32_t phys_addr);
void enable_paging_flag(void);
void set_stack_top_4mb(void);
#endif