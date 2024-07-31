#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "fat.h"
#include "disk.h"
#include "x86.h"
#include "memdef.h"
#include "memory.h"
#include "string.h"

uint8_t* kernel_mem=LOAD_KERNEL_ADDR;
uint8_t* kernel=KERNEL_LOAD_SIZE;
typedef void (*KernelStart)();

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
   uint32_t read=0;
   /*
   fat_file* file_read=open_fat(&disk, "/");
   int n=0;
   directory_entry entry;
   
   
   while (fat_entry(&disk, file_read, &entry) ){ //this function is for reading directories 
      printf("\n");
      for (int i=0; i<11; i++){
         putc(entry.filename[i]);
      }
      
      n++;
      if (n==6){
         break;
      }
   }
   close_file(file_read);
   
   char buffer[32];
   read=0;
   file_read=open_fat(&disk, "/test.txt");
   while((read=read_fat_file(&disk, file_read, sizeof(buffer),buffer))){ // this function is for reading file contents (reading dirs with this will cause unexpected behaviour)
      for (uint32_t i=0; i<read; i++){
         
         putc(buffer[i]);
         
         
      }
   }
   close_file(file_read);*/
   fat_file* kernel_read=open_fat(&disk, "/kernel.bin");
   read=0;
   uint8_t* kernel_membuf=kernel;

   while((read=read_fat_file(&disk, kernel_read, MEMORY_SIZE,kernel_mem))){ // this function is for reading file contents (reading dirs with this will cause unexpected behaviour)
      memcpy(kernel_membuf, kernel_mem, read);
      kernel_membuf+=read;
   }
   
   close_file(kernel_read);
   KernelStart kernels= (KernelStart)kernel;
   kernels();
   
end:
   while(1);
}
