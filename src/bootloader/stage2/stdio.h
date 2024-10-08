#pragma once

void clear_screen();
void putc(char c);
void puts(char* str);
void printf(const char* fstr, ...);
void update_cursor(int x,int y);
void print_buffer(const char* msg, const void* buffer, uint32_t count);