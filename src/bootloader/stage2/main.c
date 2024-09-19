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
#include "jmp.h"
uint8_t* kernel_mem=(uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* kernel=(uint8_t*)KERNEL_ADDR_PHYS;

typedef void (*KernelStart)(uint16_t boot_drive);

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
   detect_mem();
   uint32_t read=0;
   
   KernelStart kernel_e;
   /*
   if (read_elf(&disk, "/kernel.elf", (void**)&kernel_e)==false){
      printf("failed reading elf kernel");
      return;
   }*/

   // load kernel
      
      fat_file* kernel_read=open_fat(&disk, "/kernel.bin");
      read=0;
      uint8_t* kernel_membuf=kernel;
      
      while((read=read_fat_file(&disk, kernel_read, MEMORY_SIZE,kernel_mem))){ // this function is for reading file contents (reading dirs with this will cause unexpected behaviour)
         memcpy(kernel_membuf, kernel_mem, read);
         kernel_membuf+=read;
      }

      //close_file(kernel_read);
      /*
      if (read_elf(&disk, "/kernel.elf", (void**)&kernel_e)==false){
         printf("failed reading elf kernel");
      return;}*/


      //printf("adr kernel_entry: %p",kernel_e);
      //print_buffer("before paging: ", 0xc01025bd, 100);
      
      //__asm__ volatile("hlt");
     
      prepare_paging();
      //change_stack();
    
      KernelStart kernels= (KernelStart)0xc0000000+KERNEL_ADDR_PHYS;
      kernels(boot_drive);
    
    
  
   //return;
end:
   
   while(1);
}
