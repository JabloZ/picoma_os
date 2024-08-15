#pragma once

void clear_screen();
void putc(char c);
void puts(char* str);
void printf(const char* fstr, ...);
void update_cursor(int x, int y);
int video_x;
int video_y;
void remove_char(int x, int y);