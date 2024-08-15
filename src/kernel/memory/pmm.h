#include "../clibs/stdbool.h"
#include "../clibs/stdint.h"
#include "../../glibs/memory_params.h"

#define PAGE_SIZE 4096
#define MAX_BLOCKS 32768 //locked for 128mb

uint32_t* pmm_blocks[MAX_BLOCKS];
uint32_t pmm_top;
typedef struct{
    bool free;
    uint32_t size;
    struct pmm_mem_block* next_block;

} pmm_mem_block;
void init_pmm();
void* allocate_block();
void free_block();