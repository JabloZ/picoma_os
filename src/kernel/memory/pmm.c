#include "pmm.h"
#include "stdio.h"

static inline void set_bit(uint32_t bit) {
    physical_bitmap[bit / 8] |= (1 << (bit % 8));
}

static inline void clear_bit(uint32_t bit) {
    physical_bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static inline int test_bit(uint32_t bit) {
    return physical_bitmap[bit / 8] & (1 << (bit % 8));
}

void init_pmm(){
    for (int i=0; i<total_pages; i++){
        physical_bitmap[i]=0;
    }
    
      
}
void* pmm_alloc(){
    
    for (int i=0; i<total_pages; i++){
        
        if (!test_bit(i)){
            
            set_bit(i);
           
            return (void*)(&physical_bitmap +(i*PAGE_SIZE));
        }
    }
    return NULL;
}
void print_pmm(){
    
    for (int i=4000; i<4096; i++){
        printf("\nblock index: %d block: %p", i,physical_bitmap[i]);
        
    }
}
