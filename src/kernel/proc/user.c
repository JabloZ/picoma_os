#include "user.h"

tss_entry_struct tss_entry;
void usermode(){
    enter_usermode();
    
    //printf("not");
}
void test_user_function(){
    test2();
}
/*
void write_tss(gdt_entry* ge){
    uint32_t base=&tss_entry;
    uint32_t limit=sizeof(tss_entry);
    
    tss_entry.ss0=0x10;
    tss_entry.esp0=KERNEL_STACK_TOP;
    flush_tss();
    //enter_usermode();
}
void test_user_function(){
    printf("hello");
}*/