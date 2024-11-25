#ifndef STDIO_H
#define STDIO_H
#include "stdint.h"
void clear_screen();
void putc(char c);
void puts(char* str);
void printf(const char* fstr, ...);
void update_cursor(int x,int y);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
uint64_t __udivdi3(uint64_t n, uint64_t d);
uint64_t __umoddi3(uint64_t n, uint64_t d);

#endif