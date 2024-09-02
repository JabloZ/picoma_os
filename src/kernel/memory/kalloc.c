//NOTE: SHOULD RENAME THIS, THIS IS JUST BUDDY ALLOCATOR
#include "pmm.h"
#include "page.h"
#include "kalloc.h"
#include "../clibs/stdio.h"
#include "../../glibs/memory_params.h"
#define MAX_LEVELS 9
#define MEMORY_SIZE 1*1024*1024
static uint8_t memory_pool[1*1024*1024];

uint32_t blocks_iter=0;
uint8_t* memory_pool_ptr;
allocator_block g_allocator;


allocator_block* create_block(const allocator_block* b_parent, uint32_t mem_size, uint32_t lvl){
    
    allocator_block* block = (allocator_block*)memory_pool_ptr;
    memory_pool_ptr += sizeof(allocator_block);

    block->parent=b_parent;
    block->buddy1=NULL;
    block->buddy2=NULL;
    block->used=0;
    block->size=(b_parent->size)/2;
    block->level=lvl-1;
    block->block_nr=save_numbers[block->level];
    save_numbers[block->level]++;
    if (block->level>0){
        block->buddy1=create_block(block, block->size, block->level);
        block->buddy2=create_block(block, block->size, block->level);
    }
    
    
    return block;
}
void test_mem(void* ptr, uint32_t size) {
    
    if (ptr == NULL) {
        printf("error allocating memory.\n");
        return;
    }

    
    memset(ptr, 0xA, size); 
    

    for (uint32_t i = 0; i < size; i++) {
        if (((uint8_t*)ptr)[i] != 0xA) {
            return;
        }
    }
    printf("memory works fine\n");
}
void init_kalloc(){
    
    
    memory_pool_ptr = memory_pool;
    memory_pool_ptr+=sizeof(allocator_block);
    
    g_allocator.used=1;
    g_allocator.parent=NULL;
    g_allocator.buddy1=NULL;
    g_allocator.buddy2=NULL;
    g_allocator.level=BS_9;
    g_allocator.size=MEMORY_SIZE;

    
    g_allocator.buddy1=create_block(&g_allocator, g_allocator.size, g_allocator.level);
    g_allocator.buddy2=create_block(&g_allocator, g_allocator.size, g_allocator.level);
    
    
    //printf("g_alloc_size: %d\n",g_allocator->buddy2->buddy2->buddy2->buddy2->buddy2->buddy2->buddy2->buddy2->buddy2->block_nr);
   
    //TESTBLOCK
   
  
    
}
void mark_lower_used_blocks(allocator_block* block, uint32_t int_used){
        if (block->level==0){
            block->used=int_used;
            return;
        }
       
        block->buddy1->used=int_used;
        mark_lower_used_blocks(block->buddy1,int_used);
        block->buddy2->used=int_used;
        mark_lower_used_blocks(block->buddy2,int_used);
    
}

void mark_higher_used_blocks(allocator_block* block, uint32_t int_used){
        if (block->level==9){
            return;
        }
       
        block->parent->used=int_used;
        mark_higher_used_blocks(block->parent,int_used);
        
}

allocator_block* find_block(uint32_t size, const allocator_block* block){
    if (block->level==0){
        
        if (block->size>=size && block->used==0){
            
            return block;
        }
    }
    if (block->size>=size && block->buddy1->size<size && block->used==0){
        return block;
    }
    if (block->level>0){
        allocator_block* find=find_block(size, block->buddy1);
        if (find==NULL){
            find=find_block(size, block->buddy2);
        }
    }
}


void* mem_allocate(uint32_t size){
    
    if (size>MEMORY_SIZE){
        printf("memory too big"); 
        return NULL;
    }

    allocator_block* found_block=find_block(size, &g_allocator);
    
    if (found_block==NULL){
        printf("couldnt find any block to allocate\n");
        return;
    }
     found_block->memory_ptr=memory_pool_ptr+(found_block->block_nr*found_block->size);
   
    found_block->used=1;
    mark_lower_used_blocks(found_block,1);
    mark_higher_used_blocks(found_block,1);
   
    //map_page(found_block->memory_ptr, found_block->memory_ptr);
    blocks[found_block->block_nr + (1 << MAX_LEVELS)] = found_block;
    //printf("memory_ptr_allocated: %p",found_block->memory_ptr);
    //blocks[found_block->block_nr+(1<<MAX_LEVELS)]=found_block;
    
   
    return found_block->memory_ptr;
}
//to do - memory free block
void change_higher_blocks(allocator_block* block){
    if (block->level==MAX_LEVELS){
        return;
    }
    if (block->buddy1->used==0 && block->buddy2->used==0){
        block->used=0;
        change_higher_blocks(block->parent);

    }
}
void memory_free(void* mem){
    allocator_block* block;
    for (int i=0; i<((1<<MAX_LEVELS)+1);i++){
        if (mem==blocks[i]->memory_ptr){
            block=blocks[i];
        }
    }
    mark_lower_used_blocks(block, 0);
    block->used=0;
    block->memory_ptr=NULL;
    change_higher_blocks(block->parent);

}
