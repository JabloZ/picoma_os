#pragma once

void clear_screen();
void putc(char c);
void puts(char* str);
void printf(char* fstr, ...);
int video_x;
int video_y;
void remove_char(int x, int y);