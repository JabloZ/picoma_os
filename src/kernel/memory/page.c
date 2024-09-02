#include "page.h"
#define PAGE_4MB_SIZE 4*1024*1024
#define PAGE_4KB_SIZE 4*1024
#define KERNEL_SPACE 0xC0000000

page_table_entry kernel_first[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry kernel_second[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));


#define VIRT_TO_PHYS(addr) ((uint32_t)(addr) - KERNEL_SPACE)
void deinitialize(uint32_t virt_addr){
    __asm__ volatile ("invlpg (%0)" : : "r" (virt_addr) : "memory");
}

void set_current_page_directory(page_directory_entry page_directory)
{
    __asm__ volatile("movl %0, %%eax\n"
                 "movl %%eax, %%cr3":: "r" (page_directory):"eax", "memory");
}

void enable_paging(page_directory_entry *pg){
   
   __asm__ volatile("mov %0, %%cr3":: "r"(pg));
   
}


void init_page(){
    
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].user = 0;
    page_directory[0].frame_addr = ((uint32_t)VIRT_TO_PHYS(kernel_first)) >> 12;
    
    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        kernel_first[i].present = 1;
        kernel_first[i].read_write = 1;
        kernel_first[i].user = 0;
        kernel_first[i].frame_addr = i;
    }
    
    uint32_t pd_index = KERNEL_SPACE >> 22;
    page_directory[pd_index].present = 1;
    page_directory[pd_index].read_write = 1;
    page_directory[pd_index].user = 0;
    page_directory[pd_index].frame_addr = ((uint32_t)VIRT_TO_PHYS(kernel_second))>>12;

     for (int j = 0; j < PAGE_TABLE_COUNT; j++) {
        kernel_second[j].present = 1;
        kernel_second[j].read_write = 1;
        kernel_second[j].user = 0;
        kernel_second[j].frame_addr = j*0x1000;
    }
    
    uint32_t t = KERNEL_SPACE+0x000000;
    
    uint32_t t2 = 0x000000;
   
    while (t2 <= 0x3FF000) {
        vmm_map_page_4kb(t, t2);
        t2 += 0x1000;
        t += 0x1000;
        
        
    }

    enable_paging(VIRT_TO_PHYS(&page_directory));
    page_directory[0].present=0; //if unlimited low memory errors happen, just delete this line
}


void *vmm_alloc_page_4kb(void *virtual_address) {
    uint32_t physical_address = pmm_alloc();
    
    if (physical_address != NULL) {
        vmm_map_page_4kb(virtual_address, physical_address);
    }
   
    return physical_address;
}
void vmm_map_page_4mb(uint32_t virtual_address, uint32_t physical_address) {

    uint32_t pd_index = ((uint32_t)virtual_address >> 22) ;
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;

   
    page_directory[pd_index].present=1;
    page_directory[pd_index].user=0;
    page_directory[pd_index].read_write=1;
    page_directory[pd_index].frame_addr=physical_address >> 22;
    
    
    printf("kernel_first: %p", physical_address);
}
void vmm_map_page_4kb(uint32_t virtual_address, uint32_t physical_address) {
   
    uint32_t pd_index = ((uint32_t)virtual_address >> 22) ;
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;
    //printf("pd index: %d, pt index: %d, phys22: %p \n",pd_index, pt_index, page_directory[pd_index].frame_addr << 12);
    if (!page_directory[pd_index].present){
         page_directory[pd_index].present=1;
        page_directory[pd_index].user=0;
        page_directory[pd_index].read_write=1;
        page_directory[pd_index].frame_addr=physical_address >> 12;
    }
    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);
    pt[pt_index].present = 1;
    pt[pt_index].read_write = 1;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = ((uint32_t)physical_address) >>12;
    pmm_alloc_addr(physical_address);
}



