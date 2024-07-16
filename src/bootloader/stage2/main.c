#include <stdint.h>
#include "stdio.h"

uint8_t __attribute__((cdecl)) _start(uint8_t boot_drive) {
   clear_screen();
   puts("aaa \n");
   for (int i=0; i<80; i++){
      puts("baaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaccccccccccccccccccccccccccccccccccccccccccccccccccccffffffffffffffffffffffffffffffff \n");
   }
   puts("aaa \n");
   char mst[]="aaan";
  printf("Hello %s!\n", "World");
   while (1);
   return 0;
}
