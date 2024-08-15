#include "pmm.h"
#include "stdio.h"
void* allocate_block(){
    if (pmm_top<=0){
        return;
    }
    pmm_top--;
    return *pmm_blocks[pmm_top];
}

void free_block(void* addr){
    if (pmm_top>=MAX_BLOCKS){
        return;
    }
    pmm_blocks[pmm_top]=addr;
    pmm_top++;
}

void init_pmm(){
    pmm_top=0;
    for (uint32_t reg=0; reg<g_memory_regs_info->region_num; reg++){
        if (g_memory_regs_info->memory_regions[reg].region_type==USABLE){
            uint32_t* from_adr=g_memory_regs_info->memory_regions[reg].base_low;
            uint32_t how_much_to_load=g_memory_regs_info->memory_regions[reg].region_low;

            //printf("Region %d: Base Address: %p, Length: 0x%p\n", reg, from_adr, how_much_to_load);
            while (how_much_to_load>=PAGE_SIZE){
                
                //printf("Allocating block at address: %p\n", (void*)from_adr);
                free_block(from_adr);
                how_much_to_load-=PAGE_SIZE;
                from_adr+=PAGE_SIZE;
                
                
            }
        }
    }
   
    //print_pmm();   
}
void print_pmm(){
    
    for (int i=32000; i<pmm_top; i++){
        printf("\nblock index: %d block: %p", i,pmm_blocks[i]);
        
    }
}
