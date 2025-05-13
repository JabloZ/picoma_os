#include "page.h"
#define PAGE_4MB_SIZE 4*1024*1024
#define PAGE_4KB_SIZE 4*1024
#define KERNEL_SPACE 0xC0000000


#define VIRT_TO_PHYS(addr) ((uint32_t)(addr) - KERNEL_SPACE)
void deinitialize(uint32_t virt_addr){
    __asm__ volatile ("invlpg (%0)" : : "r" (virt_addr) : "memory");
}


void enable_paging(page_directory_entry *pg){
   
   __asm__ volatile("mov %0, %%cr3":: "r"(pg));
   
}
void flush_tlb_single(uint32_t addr) {
   __asm__ volatile("invlpg (%0)" ::"r" (addr) : "memory");
}


void init_page(){

    uint32_t page_dir;
    //printf("%p|",page_dir);
    memset(page_directory, 0, sizeof(page_directory));
    
    page_dir=(( ((uint32_t)VIRT_TO_PHYS(kernel_zero)))|PAGE_DIR_PRESENT|PAGE_DIR_WRITABLE);
    page_directory[0]=page_dir;
    uint32_t adr=0x0;
    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        uint32_t entry=0;
        //uint32_t phys_addr=pmm_alloc();
        pmm_alloc_addr(adr);
        entry=(adr & PAGE_FRAME_ADDR)|PAGE_PRESENT|PAGE_WRITABLE;
        kernel_zero[i]= entry;
        adr+=0x1000;
    }
    
    //printf("%p",kernel_zero[1023]);
    
    page_dir=(( ((uint32_t)VIRT_TO_PHYS(kernel_first)))|PAGE_DIR_PRESENT|PAGE_DIR_WRITABLE);
    page_directory[768]=page_dir;
    adr=0x0;
    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        uint32_t entry;
        //uint32_t phys_addr=pmm_alloc();
        pmm_alloc_addr(adr);
        entry=(adr & PAGE_FRAME_ADDR)|PAGE_PRESENT|PAGE_WRITABLE;
        kernel_first[i]= entry;
        adr+=0x1000;
    }
    uint32_t pd_index = (KERNEL_SPACE >> 22)+1;
    page_dir=(( ((uint32_t)VIRT_TO_PHYS(kernel_second)))|PAGE_DIR_PRESENT|PAGE_DIR_WRITABLE);
    page_directory[769]=page_dir;
    adr=0x400000;
    for (int j = 0; j < PAGE_TABLE_COUNT; j++) {
        uint32_t entry;
        //uint32_t phys_addr=pmm_alloc();
        pmm_alloc_addr(adr);
        entry=(adr & PAGE_FRAME_ADDR)|PAGE_PRESENT|PAGE_WRITABLE;
        kernel_second[j]= entry;
        adr+=0x1000;
        //pmm_alloc_addr(j*0x1000);
    }
    
    //
    //switch_current_dir((uint32_t)VIRT_TO_PHYS(&page_directory));
    enable_paging_flag();
    switch_current_dir((uint32_t)VIRT_TO_PHYS(&page_directory[0]));

   //set_stack_top_4mb();
  
    //page_directory[0].present=0; //if unlimited low memory errors happen, just delete this line
}
/*
void* vmm_find_free_page(){

}
void *vmm_alloc_page_4kb(void *virtual_address) {
    
    uint32_t physical_address = pmm_alloc();
    
    if (physical_address != NULL) {
        vmm_map_page_4kb(virtual_address, physical_address);
    }
   
    return virtual_address;
}
void vmm_map_page_4mb(uint32_t virtual_address, uint32_t physical_address) {

    uint32_t pd_index = ((uint32_t)virtual_address >> 22) ;
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;

   
    page_directory[pd_index].present=1;
    page_directory[pd_index].user=0;
    page_directory[pd_index].read_write=1;
    page_directory[pd_index].frame_addr=physical_address >> 12;
    
    
    printf("kernel_first: %p", physical_address);
}
void vmm_map_page_4kb(uint32_t virtual_address, uint32_t physical_address) { //try to always use vmm_alloc instead of this
    uint8_t created=0;
    page_directory[0].present=1;
    uint32_t pd_index = ((uint32_t)virtual_address >> 22);
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;
    //printf("pd index: %d, pt index: %d, phys22: %p \n",pd_index, pt_index, page_directory[pd_index].frame_addr << 12);
    if (!page_directory[pd_index].present){
        page_table_entry* entry_new = (page_table_entry*)mem_allocate(sizeof(kernel_first));
        memset(entry_new, 0, PAGE_SIZE);
        page_directory[pd_index].present=1;
        page_directory[pd_index].user=0;
        page_directory[pd_index].read_write=1;
        page_directory[pd_index].frame_addr=VIRT_TO_PHYS(entry_new)>>12;
        created=1;
    }
    //printf("pd index: %d, pt index: %d, phys22: %p \n",pd_index, pt_index, page_directory[pd_index].frame_addr << 12);
    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);
    
    if (pt[pt_index].present==1){
        printf("! virtual memory region <%p> already taken, returning. !\n", virtual_address);
        
        if (created==1){
            page_directory[pd_index].present=0;
            page_directory[pd_index].user=0;
            page_directory[pd_index].read_write=0;
            page_directory[pd_index].frame_addr=0;
        }
        //page_directory[0].present=0;
        return;
        
    }
    pt[pt_index].present = 1;
    pt[pt_index].read_write = 1;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = ((uint32_t)physical_address) >>12;
    //page_directory[0].present=0;
    //pmm_alloc_addr(physical_address);
}

void vmm_unmap_page_4kb(void *virtual_address) {
    //page_directory[0].present=1;
    uint32_t pd_index = ((uint32_t)virtual_address >> 22);
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;
    //printf("pd index: %d, pt index: %d, phys22: %p \n",pd_index, pt_index, page_directory[pd_index].frame_addr << 12);
    //printf("pd index: %d, pt index: %d, phys22: %p \n",pd_index, pt_index, page_directory[pd_index].frame_addr << 12);
    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);
    pmm_free(pt[pt_index].frame_addr);
    pt[pt_index].present = 0;
    pt[pt_index].read_write = 0;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = 0;
    //page_directory[0].present=0;
}


void vmm_memory_status(){
    uint32_t track_mem=0;
    uint32_t tracked_mem_base=0;
    uint32_t tracked_mem_length=0;
    page_directory[0].present=1; 
    uint32_t free_mem=0;
    uint32_t not_free=0;
    printf("VMM STATUS: \n");
    for (int i=1; i<1024; i++){ //without first page (first 4mb)
        if (page_directory[i].present==0){
            if (track_mem==1){
                printf("region: %p end: %p\n size: (kilobytes): %d", tracked_mem_base,tracked_mem_length+tracked_mem_base, tracked_mem_length/4096);
                track_mem=0;
                tracked_mem_length=0;
            }
            free_mem+=1024;
            continue;
        }
        else{
            for (int j=0; j<1024; j++){
                
                page_table_entry* pt = (page_table_entry*)(page_directory[i].frame_addr << 12);
                if (pt[j].present==0){
                    if (track_mem==1){
                        printf("region: %p end: %p size: (kilobytes): %d\n", tracked_mem_base, tracked_mem_length+tracked_mem_base, tracked_mem_length/4096);
                        track_mem=0;
                        tracked_mem_length=0;
                    }
                    free_mem++;
                }
                else{
                    if (track_mem==0){
                        tracked_mem_base=i*0x400000+j*0x1000;
                        tracked_mem_length+=0x1000;
                    }
                    if (track_mem==1){
                        tracked_mem_length+=0x1000;
                    }
                    track_mem=1;
                    not_free++;
                }
                
            }
            
        }
    }
    printf("taken virtual memory: %d\n",not_free*0x1000);
   //page_directory[0].present=0;
}
//page_directory[0].present=1; page_directory[0].present=0; in every action after enabled paging to ensure no physical memory error because pages frame addr is phys, not virt
void unpage_first_4mb(){
    page_directory[0].present=0;
}
void page_first_4mb(){
    page_directory[0].present=1;
}*/