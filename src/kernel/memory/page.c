#include "page.h"
#include "stdio.h"
page_directory_entry page_directory[PAGE_DIR_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry first_page_table[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry second_page_table[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));


void enable_paging(page_directory_entry *pg){
   __asm__ volatile("mov %0, %%cr3":: "r"(pg));
   uint32_t cr0;
   __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; 
   __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

void init_page(){
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].user = 0;
    page_directory[0].frame_addr = ((uint32_t)&first_page_table) >> 12;

    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        first_page_table[i].present = 1;
        first_page_table[i].read_write = 1;
        first_page_table[i].user = 0;
        first_page_table[i].frame_addr = i;
    }
  
    enable_paging((uint32_t*)&page_directory); //paging now enabled;
    test_paging();

    
}

void test_paging(){

    uint32_t *test_address = (uint32_t *)0x810000;
    *test_address = 0x1000;
    if (*test_address==0x1000){
        printf("paging succesful \n");
    }

    
}