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
    //printf("OPOFS");
    root_dir.size=15*512;
    root_dir.lba_first=3;
    root_dir.is_dir=1;
    root_dir.filename;
    uint16_t cyl, head, sec;
    lba_to_chs(0, &cyl, &head, &sec);
    //printf(" %d %d %d\n",cyl, head, sec);
    /*
    file_entry save_f;
    file_entry end_file;
    //"testdir        /example3    txt"
    if(find_file_opo(disk, "testdir        /example3    txt", &root_dir, &save_f, &end_file)==true){
        printf("endfile: %s, %d, %d, %d",end_file.filename, end_file.is_dir, end_file.size, end_file.lba_first);
    }
    uint8_t buf[end_file.size];
    read_file_opo(disk,&end_file,&buf);*/
    
    
}


bool find_file_opo(DISK* disk, char* path, file_entry* file_e, file_entry* file_test, file_entry* end_file){
    //WARNING: FOR NOW '/' AT BEGINNING OF PATH IS NOT SUPPORTED, AND PATH IS ONLY ABSOLUTE (ALWAYS FROM ROOT_DIR)
    //FORMAT PATH OPERATIONS: EXTRACT CURRENT FILE/FOLDER AND CHANGE 'folder/file1' to 'file1' for searching next files 
    uint8_t buf[512];
    uint8_t file_searched[16];
    for (int num=0; num<15; num++){
        file_searched[num]=path[num];
    }
    file_searched[15]='\0';

    uint8_t path_without_file[strlen(path)-strlen(file_searched)+1];
    int num2;
    int og_num2;
    if (strlen(path)==15){
        num2=15;
        og_num2=15;
    }
    else{
        num2=16;
        og_num2=16;
    }
    for (num2; num2<strlen(path);num2++){
        path_without_file[num2-og_num2]=path[num2];
    }
    path_without_file[strlen(path_without_file)]='\0';
    int max_j;
    if (file_e->size%512==0){
        max_j=file_e->size/SECTOR_SIZE;}
    else{
        max_j=file_e->size/SECTOR_SIZE+1;
    }
 


    //READ DISK SECTOR, SAVE TO BUFFER, TEST IF FILE_TEST FILENAME IS EQUAL TO FILE_SEARCHED
    for (int j=1; j<max_j+1;j++){
        disk_read_sectors(disk,file_e->lba_first-1+j,1,&buf);
        for (int i=0; i<16; i++){//
            
            for (int k=0; k<15; k++){
                file_test->filename[k]=buf[i*32+k];
            }
  
            file_test->is_dir=(int)buf[i*32+15];
            file_test->size=0;
            file_test->lba_first=0;
            file_test->size=(int)buf[i*32+16]<<0 | (int)buf[i*32+17]<<8 | (int)buf[i*32+18]<<16 | (int)buf[i*32+19]<<24;
            file_test->lba_first=(int)buf[i*32+20]<<0 | (int)buf[i*32+21]<<8 | (int)buf[i*32+22]<<16 | (int)buf[i*32+23]<<24;
            
            if(strcmp(file_searched,file_test->filename, 15)==1 && strlen(file_test->filename)!=0 && strlen(file_searched)!=0){
                if (strlen(path)==15){
                
                     for (int k=0; k<15; k++){
                        end_file->filename[k]=buf[i*32+k];
                    }
                    end_file->is_dir=(int)buf[i*32+15];
                    end_file->size=0;
                    end_file->lba_first=0;
                    end_file->size=(int)buf[i*32+16]<<0 | (int)buf[i*32+17]<<8 | (int)buf[i*32+18]<<16 | (int)buf[i*32+19]<<24;
                    end_file->lba_first=(int)buf[i*32+20]<<0 | (int)buf[i*32+21]<<8 | (int)buf[i*32+22]<<16 | (int)buf[i*32+23]<<24;
                    printf("size: %d lba: %d",end_file->size, end_file->lba_first);
                    return true;
                }else{
                     //printf("znaleziono, %s| %d|",path_without_file, strlen(path_without_file));
                     
                    find_file_opo(disk, path_without_file, file_test, file_test, end_file);
                }
                
            }
        }
    }
    return false;
   
}
int read_file_opo(DISK* disk, file_entry* fe, uint8_t* buf){
    
    int sectors_to_read;
    uint8_t temp_buf[512];
    int copy_mod=fe->size%512;
    if (copy_mod==0){
        sectors_to_read=fe->size/SECTOR_SIZE;
    }
    else{
        sectors_to_read=(fe->size/SECTOR_SIZE)+1;
    }

    int max_b=512;
    printf("sectors to read: %d lba_first: %d  ",sectors_to_read,fe->lba_first);
    int cpy=fe->lba_first;
    for (int i=0; i<sectors_to_read; i++){
        
        disk_read_sectors(disk,cpy+i,1,&temp_buf);
        
        if (copy_mod!=0){
            if (i+1==sectors_to_read){
                    max_b=copy_mod;
                }
        }
        else{
            max_b=SECTOR_SIZE;
        }
        for (int n=0; n<max_b; n++){
            
            buf[(i*SECTOR_SIZE)+n]=temp_buf[n];
            //printf("%d ",buf[i*SECTOR_SIZE+n]);
        }
    }
    
    return 1;
}