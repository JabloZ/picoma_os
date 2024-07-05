#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F

void _start(void) {
   char* video_memory = (char *) 0xb8000;
   *video_memory = 'X';while (1);
   return ;
}
