#include "pmm.h"
#include "stdio.h"

void init_pmm(){
    last_alloc=0;
    for (int i=0; i<total_pages; i++){
        physical_mem[i]=0;
    }
    for (int i=0; i<0x100000; i+=0x1000){
        pmm_alloc_addr(i);
    }
    for (uint32_t reg=0; reg<g_memory_regs_info->region_num; reg++){
        
        uint32_t* from_adr=g_memory_regs_info->memory_regions[reg].base_low;
        if (g_memory_regs_info->memory_regions[reg].region_type!=USABLE){
            uint32_t how_much_to_load=g_memory_regs_info->memory_regions[reg].region_low;
            //printf("Region %d: Base Address: %p, Length: 0x%p\n", reg, from_adr, how_much_to_load);
            
            while (how_much_to_load>=PAGE_SIZE){
                
                 pmm_alloc_addr(from_adr);
                how_much_to_load-=PAGE_SIZE;
                from_adr+=PAGE_SIZE;
                
                
            }
        }
       
    }
    //print_pmm();

}
void pmm_alloc_addr(uint32_t phys_addr){
    uint32_t ind= phys_addr/PAGE_SIZE;
    physical_mem[ind]=1;
    return phys_addr;

}
void* pmm_alloc(){
    for (int i=0; i<total_pages; i++){
        if (physical_mem[i]==0){
            physical_mem[i]=1;
             return i*0x1000;
        }
    }
    
}

void pmm_free(uint32_t phys_addr){
    uint32_t ind=phys_addr/PAGE_SIZE;
    physical_mem[ind]=0;
}
void print_pmm(){
    int count_free=0;
    int not_free=0;
    for (int i=0; i<total_pages; i++){
       if (physical_mem[i]==0){
        count_free++;
       }
       else{
        not_free++;
       }
        
    }
    printf("usage: %d bytes / kilobytes: %d / megabytes: %d \n",(not_free)*4096,(not_free*4096)/1024, (not_free*4096)/1048576);
    //printf("free mem: %d (x2, i have no long int print xD)", (int32_t)count_free*2048);
}
