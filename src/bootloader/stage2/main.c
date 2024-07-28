#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "fat.h"
#include "disk.h"
#include "x86.h"
#include "memdef.h"

void __attribute__((cdecl)) _start(uint16_t boot_drive) {
   clear_screen();
   DISK disk;
   if (!disk_initialize(&disk, boot_drive)){
      printf("error initializing disk");
      goto end;
   }
   if (!fat_init(&disk)){
      printf("error initializing fat driver");
      
      goto end;
   }
   printf("git");
end:
   while(1);
}
