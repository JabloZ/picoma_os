#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "memdef.h"
#include "disk.h"
#include "fat.h"

static data* g_data;
static uint8_t* g_fat = NULL;


bool read_boot(DISK* disk){
    return disk_read_sectors(disk, 0, 1, g_data->BS.boot_sector_bytes);
}
bool read_fat(DISK* disk){
    return disk_read_sectors(disk, g_data->BS.boot_sector.reserved_sectors, g_data->BS.boot_sector.sectors_per_fat, g_fat);
}
bool fat_init(DISK* disk){
    g_data = FAT_ADDR;
    if (!read_boot(disk)){
        printf("reading boot sector failed\n");
        return false;
    }
    g_fat=g_data+sizeof(data);
    if (!read_fat(disk))
    {
        printf("reading fat failed\n");
        return false;
    }
    //handle root dir
    uint32_t root_dir_lba=g_data->BS.boot_sector.reserved_sectors+g_data->BS.boot_sector.sectors_per_fat*g_data->BS.boot_sector.fat_count;
    uint32_t root_dir_size=sizeof(directory_entry)*g_data->BS.boot_sector.dir_entries;

    g_data->root_directory.file_data.pos=0;
    g_data->root_directory.file_data.file_size=sizeof(directory_entry)*g_data->BS.boot_sector.dir_entries;
    g_data->root_directory.file_data.is_directory=true;
    g_data->root_directory.file_data.manage=ROOT_DIR;
    g_data->root_directory.first_cluster=root_dir_lba;
    g_data->root_directory.current_cluster=root_dir_lba;
    g_data->root_directory.current_sector=0;
    g_data->root_directory.opened=true;
    
    if(!disk_read_sectors(disk, root_dir_lba, 1, g_data->root_directory.mem)){
        printf("failed reading root dir\n");
        return false;
    }
    printf("root directory loaded succesfully");
    return true;
};