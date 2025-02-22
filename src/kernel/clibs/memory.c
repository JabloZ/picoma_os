#include "memory.h"


void* memcpy(void* dest,const void* src, uint32_t dest_size){
    const uint8_t* copy_src=(const uint8_t*)src;
    uint8_t* copy_dest=(uint8_t*)dest;
    for (int i=0; i<dest_size; i++){
        
        copy_dest[i]=copy_src[i];
    }
    return dest;
};
void* memset(void* dest, int val, uint32_t dest_size){
    uint8_t* to_set=(uint8_t*)dest;
    for(uint32_t i=0; i<dest_size; i++){
        to_set[i]=(uint8_t)val;
    }
    return dest;
}
int memcmp(const void* ptr1, const void* ptr2, uint16_t size){
    const uint8_t* copy_1=ptr1;
    const uint8_t* copy_2=ptr2;
    for (uint16_t i=0; i<size; i++){
        if (copy_1[i]!=copy_2[i]){
            return 1;
        }

    }
    return 0;
}
void sleep(unsigned int milliseconds) {
    volatile unsigned int count;
    const unsigned int loop_per_ms = 1000;
  
    for (unsigned int i = 0; i < milliseconds; i++) {
        for (count = 0; count < loop_per_ms; count++) {
            __asm__ __volatile__("nop");
        }
    }
}

