#include <stdint.h>
#include "x86.h"


const uint32_t SCREEN_WIDTH=80;
const uint32_t SCREEN_HEIGHT=25;
const uint8_t DEFAULT_COLOR=0x7;
int8_t* video_buffer = (int8_t *) 0xb8000;
int video_x=0;
int video_y=0;

void clear_screen(){
    int8_t* video_memory = (int8_t *) 0xb8000;
    for (int i=0; i<SCREEN_HEIGHT; i++){
        for (int j=0; j<SCREEN_WIDTH; j++){
            *video_memory=' ';
            video_memory+=2;
        }
    }
    update_cursor(0,0);
    video_x=0;
    video_y=0;
}
void update_cursor(int x, int y)
{
    
	uint16_t pos = y * SCREEN_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
void put_char(int x, int y, char c){
    video_buffer[2*(y*SCREEN_WIDTH+x)]=c;
}
void put_color(int x, int y, uint8_t color){
    video_buffer[2*(y*SCREEN_WIDTH+x)+1]=color;
}
char get_char(int x, int y){
    return (char)video_buffer[2*(y*SCREEN_WIDTH+x)];
}
char get_color(int x, int y){
    return video_buffer[2*(y*SCREEN_WIDTH+x)+1];
}
void putc(char c){
    switch (c){
        case '\n':
            video_y++;
            video_x=0;
            update_cursor(video_x, video_y);
            break;
        case '\r':
            video_x = 0;
            update_cursor(video_x, video_y);
            break;
        default:
            put_char(video_x, video_y, c);
            video_x+=1;
            break;
    }
    
    if (video_x>=SCREEN_WIDTH){
        video_x-=SCREEN_WIDTH;
        video_y++;
    }
    if (video_y>=SCREEN_HEIGHT){
        scroll();
    }
    update_cursor(video_x, video_y);

}
void puts(char* str){
    while (*str){
        putc(*str);
        str++;
    }
}
void scroll(){
    for (int y=1; y<SCREEN_HEIGHT; y++){
        for (int x=0; x<SCREEN_WIDTH; x++){
            put_char(x, y-1, get_char(x, y));
            put_color(x, y-1, get_color(x, y));
        }
    }
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        put_char(x, SCREEN_HEIGHT - 1, 0);
        put_color(x, SCREEN_HEIGHT - 1, DEFAULT_COLOR);
    }
    video_y-=1;
}

typedef char* va_list;
#define va_start(ap, last) (ap = (va_list)&last + sizeof(last))
#define va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = (va_list)0)
//short and long not supported (at least for now)
#define P_STATE_NORMAL 0
#define P_STATE_SPECIFY 1

const char g_HexChars[] = "0123456789abcdef";



void printf_unsigned(unsigned long long number, int radix)
{
    char buffer[32];
    int pos = 0;
    do 
    {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = g_HexChars[rem];
    } while (number > 0);

    while (--pos >= 0)
        putc(buffer[pos]);
}

void printf(const char* fstr, ...){
    va_list args;
    va_start(args, fstr);
    int number=0;
    int radix=10;
    int state=P_STATE_NORMAL;
    while (*fstr){
        switch (state){
            case P_STATE_NORMAL:
                if (*fstr=='%'){
                    state=P_STATE_SPECIFY;
                    break;
                }
                else{
                    putc(*fstr);
                    break;
                }
                
            case P_STATE_SPECIFY:
               
                switch (*fstr){
                    case 's':  
                        puts(va_arg(args, const char*));
                        break;
                    case 'c':
                        putc(va_arg(args, int));
                        break;
                   
                    case 'd':
                        number=1; 
                        radix=10;
                        printf_unsigned(va_arg(args, int), radix);
                        break;
                    case 'i':
                        number=1; 
                        radix=10;
                        printf_unsigned(va_arg(args, int), radix);
                        break;
                    case 'p':  
                        number = 1; 
                        radix = 16; 
                        printf_unsigned(va_arg(args, int), radix);
                        break;
                    
                    case 'u':   
                        number=1;
                        radix = 10;
                        printf_unsigned(va_arg(args, int), radix);
                        break;
                    default: 
                        break;
                }
                
           
            state=P_STATE_NORMAL;
            break;
        }

        fstr++;
         
    }
    va_end(args);
    
    
}
/*


*/