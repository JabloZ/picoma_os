#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "disk.h"
#include "x86.h"
#include "memdef.h"
#include "memory.h"
#include "string.h"
#include "memory_detection.h"
#include "elf.h"
#include "jmp.h"
#include "opofs.h"
uint8_t* kernel_mem=(uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* kernel=(uint8_t*)KERNEL_ADDR_PHYS;

typedef void (*KernelStart)(uint16_t boot_drive);

void __attribute__((cdecl)) _start(uint16_t boot_drive) {
   
   printf("Aaa");
   printf("bbb");
   clear_screen();
   printf("in stage2");
   DISK disk;
   if (!disk_initialize(&disk, boot_drive)){
      printf("error initializing disk");
      goto end;
   }
  
   /*if (!fat_init(&disk)){
      printf("error initializing fat driver");
      
      goto end;

   }*/
   
   detect_mem();
   uint32_t read=0;
   
   KernelStart kernel_e;
   /*
   if (read_elf(&disk, "/kernel.elf", (void**)&kernel_e)==false){
      printf("failed reading elf kernel");
      return;
   }*/
  init_opofs(&disk);
  file_entry root_dir;
   root_dir.size=15*512;
    root_dir.lba_first=3;
    root_dir.is_dir=1;
    root_dir.filename;
   file_entry save_f;
    file_entry end_file;
   uint8_t* kernel_membuf=kernel;
    printf("kerneL:%p|",kernel_membuf);
   
   memset(kernel_membuf,0,50000);
      if (find_file_opo(&disk, "kernel      bin",&root_dir, &save_f, &end_file)){

         read_file_opo(&disk, &end_file, kernel_membuf);
      }
      else{
         printf("couldn't find kernel.bin");
      }
      
      //memcpy(kernel_membuf, buf, 30000);
      //printf(" %p ", kernel_membuf);
      for (int i=0; i<50000; i++){
         if (kernel_membuf[i]!=0){
            //printf("%p",kernel_membuf[i]);
         }
      }

     
      prepare_paging();
      
      //printf("AFTER PAGING");
      //__asm__ volatile("hlt;");
      //change_stack();
      
      KernelStart kernels= (KernelStart)0xc0000000+KERNEL_ADDR_PHYS;
      //__asm__ volatile("hlt;");
      kernels(boot_drive);
    
    
  
   //return;
end:
   
   while(1);
}
