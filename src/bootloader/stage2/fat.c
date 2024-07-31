#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "memdef.h"
#include "disk.h"
#include "fat.h"
#include "ctype.h"
#include "memory.h"
#include "string.h"
static data* g_data;
static uint8_t* g_fat = NULL;
static uint32_t g_data_section_lba=NULL;
void reset_files(){
    
    for (int i=0; i<FILES_COUNT; i++){
        
        g_data->opened_files[i].opened=false;
    }
}
bool read_boot(DISK* disk){
    return disk_read_sectors(disk, 0, 1, g_data->BS.boot_sector_bytes);
}
bool read_fat(DISK* disk){
    
    return disk_read_sectors(disk, g_data->BS.boot_sector.reserved_sectors, g_data->BS.boot_sector.sectors_per_fat, g_fat);
}
bool fat_init(DISK* disk){
    g_data = (data*)FAT_ADDR;
    if (!read_boot(disk)){
        printf("reading boot sector failed\n");
        return false;
    }
    g_fat=(uint8_t*)g_data+sizeof(data);
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
    uint32_t root_dir_mem=(root_dir_size+g_data->BS.boot_sector.bytes_per_sector-1)/g_data->BS.boot_sector.bytes_per_sector;
    g_data_section_lba=root_dir_lba+root_dir_mem;
    reset_files();
    return true;
}


uint32_t cluster_lba(uint32_t cluster){
    //printf("gdata lba: %d",g_data_section_lba+(cluster-2)*g_data->BS.boot_sector.sectors_per_cluster);
   
    return g_data_section_lba+(cluster-2)*g_data->BS.boot_sector.sectors_per_cluster;
}

fat_file* entry_open(DISK* disk, directory_entry* dir_entry){
   
    int manage=-1;
    for (int i=0; i<FILES_COUNT && manage<0; i++){
        if (g_data->opened_files[i].opened==false){
            manage=i;
        }   
    }
    if (manage<0){
        return false;
    }
    file_data* opened_file_data=&g_data->opened_files[manage];
    
    opened_file_data->first_cluster=dir_entry->first_cluster+((uint32_t)dir_entry->first_cluster_hi_bytes<<16);
    opened_file_data->current_cluster=opened_file_data->first_cluster;

    opened_file_data->file_data.is_directory=(dir_entry->attrib & FAT_DIRECTORY)!=0;
    
    opened_file_data->file_data.file_size=dir_entry->file_size;
  
    opened_file_data->file_data.manage=manage;
    opened_file_data->file_data.pos=0;
    opened_file_data->current_sector=0;
    
    if (!disk_read_sectors(disk, cluster_lba(opened_file_data->current_cluster), 1, opened_file_data->mem)){
        printf("\nFAILED DISK READ.");
        return false;
    };
    opened_file_data->opened=true;
    


    //opened_file_data->file_data.is_directory=true; //COMMENT THAT FOR NO DIRECTORY (FOR NOW ONLY DEBUGGING)
    //opened_file_data->file_data.file_size=26;
    
    
    printf("\nAfter disk read: First cluster: %u, Is directory: %d, File size: %u, Manage: %d, Position: %u\n",
           opened_file_data->first_cluster,
           opened_file_data->file_data.is_directory,
           opened_file_data->file_data.file_size,
           opened_file_data->file_data.manage,
           opened_file_data->file_data.pos);
    return &opened_file_data->file_data;
}

fat_file* open_fat(DISK* disk, const char* path){
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
        
        
        directory_entry dir_entry;/*
        printf("dir entry filename: %s\n",dir_entry.filename);
        printf("dir entry filename: %d\n",strlen(dir_entry.filename));
        printf("dir entry attrib: %d\n",dir_entry.attrib);
        printf("dir entry first clust hi: %d\n",dir_entry.first_cluster_hi_bytes);
        printf("dir entry first cluster: %d\n",dir_entry.first_cluster);
        printf("path name: %s\n",path_name);

        printf("cf filesize: %d\n",cf->file_size);
        printf("cf pos: %d\n",cf->pos);
        printf("cf is dir: %d\n",cf->is_directory);*/
        if (dir_entry.attrib==FAT_DIRECTORY){
            //cf->file_size=32; //FOR SOME REASON IF IT IS DIRECTORY IT CANNOT ASSIGN FILE_SIZE WELL, SO WITHOUT THIS LINE IT WILL CRASH BUT OFC ITS FOR DEBUGGING ONLY
            //probably fat read issue 
            
        }

        if (find_file(disk, cf, path_name, &dir_entry)){
            printf("\nFOUND: dir entry: %s\n", dir_entry.filename);
            
            close_file(cf);
            if (end_path==false && dir_entry.attrib & FAT_DIRECTORY==0){
                printf("not dir: %s", path_name);
                return NULL;
            }
           
            cf=entry_open(disk, &dir_entry);
            //printf("After entry_open: is_directory: %d, filesize: %d, manage: %d, pos: %d\n", cf->is_directory, cf->file_size, cf->manage, cf->pos);
            //cf->is_directory=true;
        }
        else{
            printf("file: %s NOT FOUND.",path_name);
            close_file(cf);
            return NULL;
        }

        printf("OPENED: filename: %s\n", dir_entry.filename);
    }
    
    
    return cf;
}    
bool fat_entry(DISK* disk, fat_file* cf, directory_entry* dir_entry){
    
    return read_fat_file(disk,cf,sizeof(directory_entry),dir_entry)==sizeof(directory_entry);
}
bool find_file(DISK* disk, fat_file* cf, const char* name, directory_entry* dir_entry_out){
    
    directory_entry dir_entry;
   
    char file_name[12];
    memset(file_name,' ', sizeof(file_name));
    file_name[11]='\0';
    const char* extension=strchr(name,'.');
   
    if (extension==NULL){
        extension=name+11;
    }
    for (int i=0; i<8 && name[i] && name+i<extension; i++){
        file_name[i]=toupper(name[i]);
    }
    
    if (extension!=name+11){
        
        for (int i=0; i<3 && extension[i+1]; i++){
           
            file_name[i+8]=toupper(extension[i+1]); //skip dot
            
        }
        
    }
    //printf(":%s|",dir_entry_out->filename);
    //printf("\n ______________________ EVERYTHING AFTER THIS IS FOR TEST IN READ_FAT");
    while(fat_entry(disk, cf, &dir_entry)){
        
      
       
        if (memcmp(file_name, dir_entry.filename, 11) == 0)
        {
            
            //dir_entry.filename[11]='\0';
           
            *dir_entry_out = dir_entry;
            
            return true;
        }  
    }
    printf("\n%s not found!",file_name);
    return false;
}
uint32_t next_cluster(uint32_t current_cluster){
   
    uint32_t pos=current_cluster*3/2;
   
    if (current_cluster%2==0){
        
        return (*(uint16_t*)(g_fat+pos)) & 0x0FFF;
    }
    else{
        
        return (*(uint16_t*)(g_fat+pos)) >> 4;
    }


}
uint32_t read_fat_file(DISK* disk, fat_file* file, uint32_t bytes, void* data_out){
    
    file_data* file_read;
    
    uint8_t* buf_out=(uint8_t*)data_out;
    if (file->manage==ROOT_DIR){
        
        file_read=&g_data->root_directory;
    }
    else{
        
        file_read=&g_data->opened_files[file->manage];
    }/*
    printf("\nfileread data: %d",g_data->opened_files[1].opened);
    printf("\nfileread data: %d",g_data->opened_files[0].current_cluster);
    printf("\nfileread data: %d",file_read->current_cluster);
    printf("\nfileread data is dir: %d",file_read->file_data.is_directory);*/
    if (!file_read->file_data.is_directory || file_read->file_data.is_directory==true && file_read->file_data.file_size==0){
        if (file_read->file_data.file_size-file_read->file_data.pos<bytes){
           
            bytes=file_read->file_data.file_size-file_read->file_data.pos;
            
        }
    }
    
    while(bytes>0){
      
        uint32_t in_buffer=SECTOR_SIZE - (file_read->file_data.pos % SECTOR_SIZE);
       
        uint32_t to_take=bytes;
        
        if (to_take<in_buffer){
            to_take=to_take;
        }
        else{
            to_take=in_buffer;
        }
 
        
        //printf("Debug: in_buffer=%u, to_take=%u\n", in_buffer, to_take);
        for (uint32_t i = 0; i < to_take; i++) {
        //printf("%c ", file_read->mem[file_read->file_data.pos % SECTOR_SIZE + i]);
        }
        memcpy(buf_out, file_read->mem + file_read->file_data.pos%SECTOR_SIZE, to_take);
       
        buf_out+=to_take;
        file_read->file_data.pos+=to_take;
        bytes-=to_take;
       
        if (in_buffer==to_take){
            /*
            printf("top \n");
            printf("cf: %d\n", file_read->mem);
            printf("cf: %d\n", file_read->opened);
            printf("cf: %d\n", file_read->current_cluster);
            printf("cf: %d\n", file_read->file_data.file_size);*/
            
            if (file_read->file_data.manage==ROOT_DIR){
                
                ++file_read->current_cluster;
                if (!disk_read_sectors(disk, file_read->current_cluster, 1, file_read->mem)){
                    //printf("error reading root dir");
                    
                    break;
                }
               
            }
            else{
              
                
                
                if (++file_read->current_sector>=g_data->BS.boot_sector.sectors_per_cluster){
                    
                    
                    file_read->current_sector=0;
                    file_read->current_cluster=next_cluster(file_read->current_cluster);
                    
                }
                if (file_read->current_cluster>=0xFF8){
                    
                    file_read->file_data.file_size=file_read->file_data.pos;
                    break;
                }
                if (!disk_read_sectors(disk, cluster_lba(file_read->current_cluster)+file_read->current_sector,1, file_read->mem))
                {
                    
                    //printf("FAT: read error!\r\n");
                    break;
                }
            }
        }
    }
   
    return buf_out-(uint8_t*)data_out;

}
void close_file(fat_file* file)
{
    if (file->manage == ROOT_DIR){
        file->pos= 0;
        g_data->root_directory.current_cluster=g_data->root_directory.first_cluster;
    }
    else
    {
        g_data->opened_files[file->manage].opened = false;
    }
}