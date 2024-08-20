#include "page.h"
#include "stdio.h"

void enable_paging(page_directory_entry *pg){
   __asm__ volatile("mov %0, %%cr3":: "r"(pg));
   uint32_t cr0;
   __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; 
   __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}
page_table_entry* create_page_table(){
    int i;
    for (i=0; i<1024; i++){
        if (page_directory[i].present==0){
            break;
        }
    }
    
    page_table_entry* page_table = mem_allocate(sizeof(page_table_entry) * PAGE_TABLE_COUNT);
    page_directory[i].present = 1;
    page_directory[i].read_write = 1;
    page_directory[i].user = 0;
    
    page_directory[i].frame_addr = ((uint32_t)page_table) >> 12;
    
     for (int j = 0; j < PAGE_TABLE_COUNT; j++) {
        page_table[j].present = 1;
        page_table[j].read_write = 1;
        page_table[j].user = 0;
        page_table[j].frame_addr = j;
    }

    if (page_table == NULL) {
        printf("Error: Could not allocate page table.\n");
        return NULL;
    }
    return page_table;
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
  
    uint32_t max_mem= g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].base_low+g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].region_low;
    
    enable_paging((uint32_t*)&page_directory); //paging now enabled;
   
    //test_paging();

    
}

void test_paging(){

    uint32_t *test_address = (uint32_t *)0x1000;
    *test_address = 0x1000;
    if (*test_address==0x1000){
        printf("paging succesful \n");
    }

    
}
void map_page(void* phys_addr, void* virt_addr) {
    uint32_t pd_index = ((uint32_t)virt_addr >> 22) & 0x3FF;
    uint32_t pt_index = ((uint32_t)virt_addr >> 12) & 0x3FF;

    // Sprawdź, czy tablica stron istnieje
    if (!page_directory[pd_index].present) {
        page_directory[pd_index].present = 1;
        page_directory[pd_index].read_write = 1;
        page_directory[pd_index].user = 0;
        page_directory[pd_index].page_size = 0;
        page_directory[pd_index].frame_addr = ((uint32_t)create_page_table()) >> 12;
    }

    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);

    // Zmapuj stronę
    pt[pt_index].present = 1;
    pt[pt_index].read_write = 1;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = ((uint32_t)phys_addr) >> 12;
}