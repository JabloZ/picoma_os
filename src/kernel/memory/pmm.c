#include "pmm.h"
#include "stdio.h"

void free_memory_block(uint32_t* addr){
    if (pmm_top>=MAX_BLOCKS){
        return;
    }
    pmm_blocks[pmm_top]=*addr;
    pmm_top++;
}
void* allocate_memory_block(){
    if (pmm_top<=0){
        return;
    }
    pmm_top--;
    return *pmm_blocks[pmm_top];
}
void init_pmm(){
    pmm_top=0;
    for (uint32_t entry=0; entry<g_memory_regs_info->region_num; entry++){
        //g_memory_regs_info->memory_regions[entry];
        if (g_memory_regs_info->memory_regions[entry].region_type==USABLE){
            uint32_t* addr=g_memory_regs_info->memory_regions[entry].base_low;
            uint32_t length=g_memory_regs_info->memory_regions[entry].region_low;
           
            while (length>=PAGE_SIZE){
                 
                free_memory_block(*addr);
                length-=PAGE_SIZE;
                addr+=PAGE_SIZE;
            }
        }
    }
    //printf("succesfully initiliazed pmm");
    
}
