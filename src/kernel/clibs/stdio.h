#pragma once
#include "stdint.h"
void clear_screen();
void putc(char c);
void puts(char* str);
void printf(char* fstr, ...);
void update_cursor(int x, int y);
int video_x;
int video_y;
void remove_char(int x, int y);
void init_vals();
int8_t* video_buffer;
int8_t* video_memory;
uint32_t SCREEN_WIDTH;
uint32_t SCREEN_HEIGHT;
uint8_t DEFAULT_COLOR;