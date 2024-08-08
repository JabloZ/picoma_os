#include "memory_detection.h"

void detect_mem(){
    
    memory_region mem_r;
    uint32_t next_id=0;
    int next_ret;
    int iter_mem_regs=0;
    next_ret=x86_call_E820(&mem_r, &next_id);
    
    while(next_id!=0 && next_ret>0){
        g_mem_regs[iter_mem_regs].base_low=mem_r.base_low;
        //ignore base high
        g_mem_regs[iter_mem_regs].region_low=mem_r.region_low;
        //ignore region high
        g_mem_regs[iter_mem_regs].region_type=mem_r.region_type;
        g_mem_regs[iter_mem_regs].acpi=mem_r.acpi;
        
        printf("E820: base=%p length=%p type=%p acpi=%p\n", mem_r.base_low, mem_r.region_low, mem_r.region_type, mem_r.acpi);
        next_ret = x86_call_E820(&mem_r, &next_id);
        iter_mem_regs++;
    }
    g_memory_regs_info->memory_regions=g_mem_regs;
    g_memory_regs_info->region_num=iter_mem_regs;
    printf("mem reg: %d",g_memory_regs_info->region_num);
    printf("address %p", g_memory_regs_info);
}