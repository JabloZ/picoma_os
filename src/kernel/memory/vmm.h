#include "../clibs/stdbool.h"
#include "../clibs/stdint.h"
#include "../clibs/memory.h"
#include "../../glibs/memory_params.h"
typedef enum{ //*4096
    BS_0=0, //4KB
    BS_1,
    BS_2,
    BS_3,
    BS_4,
    BS_5,
    BS_6,
    BS_7,
    BS_8,
    BS_9,
    BS_10,
    BS_11
} buddy_sizes;


typedef struct allocator_block {
    struct allocator_block* parent;
    struct allocator_block* buddy1;
    struct allocator_block* buddy2;
    uint32_t level;
    uint8_t used;
    uint32_t size;
    uint8_t* memory_ptr;
} allocator_block;

void* mem_allocate(uint32_t size);
allocator_block* create_block(const allocator_block* b_parent, uint32_t mem_size, uint32_t lvl);
void* mem_allocate(uint32_t size);
allocator_block* find_block(uint32_t size, const allocator_block* block);
void mark_higher_used_blocks(allocator_block* block, uint32_t int_used);
void mark_lower_used_blocks(allocator_block* block, uint32_t int_used);