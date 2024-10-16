#include "opofs.h"
#define MAX_OPENED_FILES 16
#define SECTOR_SIZE 512
#define ROOT_SECTORS 15

file_entry root_dir;

int lba_to_chs(int lba, uint16_t* cyl, uint16_t* head, uint16_t* sec){
    *cyl=lba/(HEAD*SECTOR);
    lba=lba%(HEAD*SECTOR);
    *head=lba/SECTOR;
    lba=lba%SECTOR+1;
    *sec=lba;
}

void init_opofs(DISK* disk){
    printf("OPOFS");
    root_dir.size=15*512;
    root_dir.lba_first=4;
    root_dir.is_dir=1;
    root_dir.filename;
    uint16_t cyl, head, sec;
    lba_to_chs(0, &cyl, &head, &sec);
    printf(" %d %d %d\n",cyl, head, sec);
    find_file_opo(disk, "example     txt", &root_dir);
}


uint32_t find_file_opo(DISK* disk, char* path, file_entry* file_e){
    uint8_t buf[512];
    uint8_t file_searched[15];
    for (int num; num<15; num++){
        file_searched[num]=path[num];
    }
    uint8_t path_without_file[strlen(path)];
    for (int num2=0; num2<strlen(path)-15;num2++){
        path_without_file[num2]=path[num2];
    }
    
    file_entry file_test;
    for (int j=1; j<15+1;j++){
        disk_read_sectors(disk,4-1+j,1,&buf);
        
        for (int i=0; i<16; i++){//
        
            for (int k=0; k<15; k++){
                file_test.filename[k]=buf[i*32+k];
            }

            file_test.is_dir=buf[i*32+15];
            file_test.size=buf[i*32+16]<<0 | buf[i*32+17]<<8 | buf[i*32+18]<<16 | buf[i*32+19]<<24;
            file_test.lba_first=buf[i*32+20]<<0 | buf[i*32+21]<<8 | buf[i*32+22]<<16 | buf[i*32+23]<<24;

            printf("FILE INFO: filename:%s, filesize:%d, file_lba:%d, is_dir:%d\n",file_test.filename, file_test.size, file_test.lba_first,file_test.is_dir);
            
        }
    }
    return 111;
}
