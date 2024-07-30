#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "fat.h"
#include "disk.h"
#include "x86.h"
#include "memdef.h"
#include "memory.h"
#include "string.h"

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
   fat_file* file_read=open_fat(&disk, "/");
   int n=0;
   directory_entry entry;
   uint32_t read=0;
   /*
   while (fat_entry(&disk, file_read, &entry) ){
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
   
   
  
   char buf[100];
   

   file_read=open_fat(&disk, "/test.txt");
   printf("ODCZYTANY PLIK: \n");
   while((read=read_fat_file(&disk, file_read, sizeof(buf),buf))){
      for (uint32_t i=0; i<read; i++){
         
         putc(buf[i]);
         n++;
         
      }
   }*/
   close_file(file_read);
   char buffer[100];
   read=0;
   file_read=open_fat(&disk, "testd/testd2/test.txt");
   while((read=read_fat_file(&disk, file_read, sizeof(buffer),buffer))){
      for (uint32_t i=0; i<read; i++){
         
         putc(buffer[i]);
         n++;
         
      }
   }
   close_file(file_read);
end:
   while(1);
}
