#include "pmm.h"
void init_vmm(){
    uint32_t* vmm_kernel_address=allocate_block();
    memset(vmm_kernel_address, 0, PAGE_SIZE);
    
}


 