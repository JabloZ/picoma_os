#include "opofs.h"
#define MAX_OPENED_FILES=16;
#define SECTOR_SIZE=512;
int lba_to_chs(int lba, uint16_t* cyl, uint16_t* head, uint16_t* sec){
    *cyl=lba/(HEAD*SECTOR);
    lba=lba%(HEAD*SECTOR);
    *head=lba/SECTOR;
    lba=lba%SECTOR+1;
    *sec=lba;
}

void init_opofs(DISK* disk){
    printf("OPOFS");
    uint16_t cyl, head, sec;
    lba_to_chs(0, &cyl, &head, &sec);
    printf(" %d %d %d\n",cyl, head, sec);
    int main_dir_lba=4;
    uint8_t buf[512];
    disk_read_sectors(disk,4,1,&buf);
    for (int i=0; i<512; i++){
        printf("%p ",buf[i]);
    }
    //main dir chs(0,0,4) - start

}
