#include "elf.h"
void read_elf(char* filename_to_read){
    
    
    //bool find_file_opo(uint32_t disk, char* path, file_entry* file_e, file_entry* file_test, file_entry *end_file, int* lba_of_filedir);
    //int read_file_opo(uint32_t disk, file_entry* fe, uint8_t* buf);
    file_entry f1;
    file_entry f2;
    file_entry save_f;
    file_entry file_e;
    uint8_t buf[50000];
    return_file_entry_with_name("stage2.bin",&save_f,&file_e);
    printf("%s",file_e.filename);
    read_file_opo(0,&file_e,&buf);
    printf("%d %d %d %d",buf[0],buf[1],buf[2],buf[3]);
    memory_free(buf);
    printf("no err");
}