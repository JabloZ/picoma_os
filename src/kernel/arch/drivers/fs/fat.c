#include "fat.h"
#include "../fdc.h"
static uint32_t g_fat_type = 12;
static data* g_data;
uint8_t* g_fat=NULL;



int read_boot_sector(){
    return fdc_read_sectors(0,0,1,g_data->BS.boot_sector_bytes);
}   
int read_fat(){
    return fdc_read_sectors(0,g_data->BS.boot_sector.reserved, g_data->BS.boot_sector.sectors_per_fat, g_fat);
}
void init_fat(){

    if (!read_boot_sector()){
        printf("reading boot sector failed!");
        return;
    }
     if (!read_fat()){
        printf("reading fat failed!");
        return;
    }
    uint32_t g_total_sectors=g_data->BS.boot_sector.total_sectors;
    uint32_t root_dir_lba=g_data->BS.boot_sector.reserved_sectors+g_data->BS.boot_sector.sectors_per_fat*g_data->BS.boot_sector.fat_count;
    uint32_t root_dir_size=g_data->BS.boot_sector.dir_entries*sizeof(directory_entry);

    g_data->root_directory.current_cluster=root_dir_lba;
    g_data->root_directory.first_cluster=root_dir_lba;
    g_data->root_directory.current_sector=0;
    g_data->root_directory.file_data.file_size=sizeof(directory_entry)*g_data->BS.boot_sector.dir_entries;
    g_data->root_directory.file_data.is_directory=true;
    g_data->root_directory.file_data.manage=ROOT_DIR;
    g_data->root_directory.file_data.pos=0;
    g_data->root_directory.opened=true;
    
    if (!fdc_read_sectors(0,root_dir_lba,1,g_data->root_directory.mem)){
        printf("reading root directory failed!");
        return;
    }
   
    uint32_t root_dir_length=(root_dir_size+g_data->BS.boot_sector.bytes_per_sector-1)/g_data->BS.boot_sector.bytes_per_sector;
    uint32_t g_data_section_start_lba=root_dir_length+root_dir_lba;
}
uint32_t cluster_lba(uint32_t cluster){
    //printf("gdata lba: %d",g_data_section_lba+(cluster-2)*g_data->BS.boot_sector.sectors_per_cluster);
    //return g_data_section_lba+(cluster-2)*g_data->BS.boot_sector.sectors_per_cluster;
}
fat_file* open_fat(uint32_t disk, const char* path){
      char path_name[256];
    if (path[0]=='/'){
        path++;
    }
   
    fat_file* cf=&g_data->root_directory.file_data;
    
    while (*path){
     
        bool end_path=false;
        const char* bslash_name=strchr(path, '/'); //char between '/''s
        
        if (bslash_name!=NULL){
            memcpy(path_name, path, bslash_name-path);
            
            path_name[bslash_name-path]='\0';
            path=bslash_name+1;
            end_path=false;

        }
        else{
           
            end_path=true;
            unsigned len=strlen(path);
            memcpy(path_name, path, len);
            path_name[len+1]='\0';
            path+=len;

        }
        
    directory_entry dir_entry;
    
    }
}
