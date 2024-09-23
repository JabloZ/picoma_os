#include "mbr.h"
void* segment_to_linear(void* addr)
{
    uint32_t offset = (uint32_t)(addr) & 0xFFFF;
    uint32_t segment = (uint32_t)(addr) >> 16;

    return (void*)(segment * 16 + offset);
}
void mbr_detect_partition(Partition* partition, DISK* disk, void* out){
    partition->disk=disk;
    if (disk->id<0x80){ //not bootable
        partition->partition_offset=0;
        partition->partition_size=disk->cylinders * disk->heads * disk->sectors;
    }
    else{
        Mbr_entry* mbr_entry=segment_to_linear(partition);
        partition->partition_offset=mbr_entry->lba_start;
        partition->partition_size=mbr_entry->sectors_num;
    }
}