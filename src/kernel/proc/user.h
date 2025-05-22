#ifndef USER_H
#define USER_H
#include "stdint.h"
#include "../arch/gdt/gdt.h"
#include "../clibs/memory.h"
#include "../memory/page.h"  
#define USER_STACK_TOP 0x003FFFFF
void enter_usermode(void);
void test_user_function();
void usermode();
void test_user_function();
void write_tss(gdt_entry* ge);
void flush_tss(void);
void test2();
#endif