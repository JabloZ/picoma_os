#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "fat.h"
#include "disk.h"
#include "x86.h"
#include "memdef.h"
#include "memory.h"
#include "string.h"
#include "memory_detection.h"
#include "elf.h"
uint8_t* kernel_mem=(uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* kernel=(uint8_t*)KERNEL_LOAD_ADDR;
typedef void (*KernelStart)(void);

void __attribute__((cdecl)) _start(uint16_t boot_drive) {
   clear_screen();
   DISK disk;
   printf("\nDISK INFO: %p", &disk);
   if (!disk_initialize(&disk, boot_drive)){
      printf("error initializing disk");
      goto end;
   }
  
   if (!fat_init(&disk)){
      printf("error initializing fat driver");
      
      goto end;

   }
   detect_mem();
   uint32_t read=0;
   
   KernelStart kernel_e;
   if (read_elf(&disk, "/kernel.elf", (void**)&kernel_e)==false){
      printf("failed reading elf kernel");
      return;
   }
   
  
    printf("Execute kernel at address: %p\n", (void*)kernel_e);
  
    KernelStart kernels = kernel_e;
    kernels();
   //return;
end:
   printf("w end");
   while(1);
}
