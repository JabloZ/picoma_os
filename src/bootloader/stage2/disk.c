#include "x86.h"
#include "disk.h"
#include "stdio.h"
#include "string.h"
/*
    uint8_t id;
    uint16_t cylinders;
    uint16_t sectors;
    uint16_t heads;
    DISK;
*/

bool disk_initialize(DISK* disk, uint8_t drive){
    
    uint8_t type;
    uint16_t cylinders, sectors, heads;
    if (!x86_disk_parameters(disk->id, &type ,&cylinders, &sectors, &heads)){
        return false;
    }
    disk->id=drive;
    disk->cylinders=cylinders;
    disk->sectors=sectors;
    disk->heads=heads;
    /*
    printf("type: %d\n",type);
    printf("sectors: %d\n", disk->sectors);
    printf("heads: %d\n", disk->heads);
    printf("cylinders: %d\n", disk->cylinders);*/
    return true;
};
/*
;lba to chs formula:
;C = LBA ÷ (HPC x SPT)
;H = (LBA ÷ SPT) mod HPC
;S = (LBA mod SPT) + 1
*/
int disk_lba_to_chs(DISK* disk, uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sec){
    *cyl=lba/(HEAD*SECTOR);
    lba=lba%(HEAD*SECTOR);
    *head=lba/SECTOR;
    lba=lba%SECTOR+1;
    *sec=lba;
    
}


//bool x86_read_disk(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t sectors_count, void* dataOut);
bool disk_read_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void *data_out){
    uint16_t cylinder, sector, head;
    
    disk_lba_to_chs(disk, lba, &cylinder, &head,  &sector);
    //printf("chs: %d %d %d ",cylinder, head, sector);
    for (int i=0; i<3; i++){
            
        if(x86_read_disk(disk->id, cylinder, sector, head, sectors, data_out)){
            return true;
        }
       
        x86_reset_disk(disk->id);
        
    }
    
    return false;
};