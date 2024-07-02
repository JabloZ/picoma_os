#include <stdint.h>

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F

void _start(void) {
    volatile uint16_t* video_memory = (uint16_t*) VIDEO_MEMORY;
    uint16_t value = (WHITE_ON_BLACK << 8) | 'E';
    video_memory[0] = value;

    while (1) { }
}
