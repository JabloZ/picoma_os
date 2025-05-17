#include "elf.h"
void read_elf(char* filename_to_read){
    
    
    //bool find_file_opo(uint32_t disk, char* path, file_entry* file_e, file_entry* file_test, file_entry *end_file, int* lba_of_filedir);
    //int read_file_opo(uint32_t disk, file_entry* fe, uint8_t* buf);
    file_entry f1;
    file_entry f2;
    file_entry save_f;
    file_entry file_e;
    
    return_file_entry_with_name("kernel.bin",&save_f,&file_e);
    uint8_t* buf=mem_allocate(file_e.size);
    //memset(buf,0,file_e.size);
    
    //printf("%s",file_e.filename);
    read_file_opo(0,&file_e,buf);
        for (int i=0; i<file_e.size; i++){
        printf("%c",buf[i]);
        printf("%d",i);
    }

    memory_free(buf);
    
}