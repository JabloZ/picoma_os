//NOTE: SHOULD RENAME THIS, THIS IS JUST BUDDY ALLOCATOR
#include "kalloc.h"

#define MAX_LEVELS 10
#define MEMORY_SIZE 4*1024*1024
allocator_block memory_pool[(1<<BS_10+1)];

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
    block->block_adr=NULL;
    block->page_first=0;
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
    
    taken_blocks=0;
    memory_pool_ptr = memory_pool;
    memory_pool_ptr+=sizeof(allocator_block);
    
    g_allocator.used=1;
    g_allocator.parent=NULL;
    g_allocator.buddy1=NULL;
    g_allocator.buddy2=NULL;
    g_allocator.level=BS_10;
    g_allocator.size=MEMORY_SIZE;

    
    g_allocator.buddy1=create_block(&g_allocator, g_allocator.size, g_allocator.level);
    g_allocator.buddy2=create_block(&g_allocator, g_allocator.size, g_allocator.level);
    vmm_alloc_page_4mb(kalloc_memory, 0xC0800000, VIRT_TO_PHYS(kalloc_memory));
   
    
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
   
     
   
    
    found_block->used=1;
    
    found_block->page_first=taken_blocks;
    found_block->block_adr=0xC0800000+taken_blocks*0x1000;
    for (int i=0; i<found_block->size; i+=4096){
        
        int* ptr=pmm_alloc(4096);
        vmm_alloc_page_4kb(kalloc_memory,0xC0800000+taken_blocks*0x1000,ptr);
        
        flush_tlb_single(0xC0800000+taken_blocks*0x1000);
        taken_blocks+=1;
    }
    mark_lower_used_blocks(found_block,1);
    mark_higher_used_blocks(found_block,1);
   
    //map_page(found_block->memory_ptr, found_block->memory_ptr);
    blocks[found_block->block_nr] = found_block;
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
    
    int count=0;
    for (int i=0; i<((1<<(MAX_LEVELS)));i++){
        if (mem==blocks[i]->memory_ptr){
            count++;

            block=blocks[i];
            break;
        }
    }
    if(count==0){
        printf("\nsomething went wrong - no memory freed\n");
        return;
    }
    
    //block=mem;
    uint32_t start_block=block->memory_ptr;
    
    for (int i=block->size; i>0; i-=4096){
        //printf("%p",start_block);
        //pmm_free(start_block);
        vmm_unmap_page_4kb(kalloc_memory, start_block);
        start_block+=0x1000;
        taken_blocks-=1;
    }
    mark_lower_used_blocks(block, 0);
    block->used=0;
    block->memory_ptr=NULL;
    change_higher_blocks(block->parent);
    
}
