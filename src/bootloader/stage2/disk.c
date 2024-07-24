#include "x86.h"
#include "disk.h"

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
    return true;
};
/*
;lba to chs formula:
;C = LBA ÷ (HPC x SPT)
;H = (LBA ÷ SPT) mod HPC
;S = (LBA mod SPT) + 1
*/
void disk_lba_to_chs(DISK* disk, uint32_t lba, uint16_t* _cylinder, uint16_t* _sector, uint16_t* _head){
    *_sector=(lba%disk->sectors)+1;

    *_head=(lba/disk->sectors)%disk->heads;
    
    *_cylinder=lba/(disk->sectors*disk->heads);
    
}

//bool x86_read_disk(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t sectors_count, void* dataOut);
bool disk_read_sectors(DISK* disk, uint32_t lba, uint8_t sectors, void *data){
    uint16_t cylinder, sector, head;
    disk_lba_to_chs(disk, lba, &cylinder, &sector, &head);
    for (int i=0; i<3; i++){
        if(!x86_read_disk(disk->id, cylinder, sector, head, sectors, data)){
            return true;
        }
        x86_reset_disk(disk->id);
    }
    return false;
};