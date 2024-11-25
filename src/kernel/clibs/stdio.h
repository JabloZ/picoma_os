#ifndef STDIO_H
#define STDIO_H
#include "stdint.h"
#include "x86.h"

void clear_screen();
void putc(char c);
void puts(char* str);
void printf(char* fstr, ...);
void scroll();
void update_cursor(int x, int y);
int video_x;
int video_y;
void remove_char(int x, int y);
uint64_t __udivdi3(uint64_t n, uint64_t d);
uint64_t __umoddi3(uint64_t n, uint64_t d);
void init_vals();
uint8_t* global_video_memory_start;
int8_t* video_buffer;
int8_t* video_memory;
uint32_t SCREEN_WIDTH;
uint32_t SCREEN_HEIGHT;
uint8_t DEFAULT_COLOR;
#endif