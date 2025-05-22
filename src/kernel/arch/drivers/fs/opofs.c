#include "opofs.h"

#define MAX_OPENED_FILES 16
#define SECTOR_SIZE 512
#define ROOT_SECTORS 15



int lba_to_chs(int lba, uint16_t* cyl, uint16_t* head, uint16_t* sec){
    *cyl=lba/(HEAD*SECTOR);
    lba=lba%(HEAD*SECTOR);
    *head=lba/SECTOR;
    lba=lba%SECTOR+1;
    *sec=lba;
}

void init_opofs(uint32_t disk){
    //printf("OPOFS");
    root_dir.size=15*512;
    root_dir.lba_first=3;
    root_dir.is_dir=1;
    root_dir.filename;
    uint16_t cyl, head, sec;
    lba_to_chs(0, &cyl, &head, &sec);
    //printf(" %d %d %d\n",cyl, head, sec);
    
    file_entry save_f;
    file_entry end_file;
    file_entry to_read;
    
    /*
    uint8_t buf1[512];
    strcpy(&to_read.filename, "example3    txt");
    to_read.is_dir=0;
    to_read.lba_first=20;
    to_read.size=512;
    
    read_file_opo(0, &to_read, &buf1);
   
    for (int i=0; i<512; i++){
        printf("%c ",buf1[i]);
    }
    return;*/
    
    
}


bool find_file_opo(uint32_t disk, char* path, file_entry* file_e, file_entry* file_test, file_entry* end_file, int* lba_of_filedir){
    //WARNING: FOR NOW '/' AT BEGINNING OF PATH IS NOT SUPPORTED, AND PATH IS ONLY ABSOLUTE (ALWAYS FROM ROOT_DIR)
    //FORMAT PATH OPERATIONS: EXTRACT CURRENT FILE/FOLDER AND CHANGE 'folder/file1' to 'file1' for searching next files
    //printf("%s, %d, %d ",path,file_e->lba_first, &lba_of_filedir);
    bool can_change_filedir=true;
    if (strlen(path)<15){
        return false;
    } 
    
    uint8_t* buf=mem_allocate(512);
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
    
    for (num2; num2<strlen(path);num2++){// od 16 do 31, str od 0 do 15
        path_without_file[num2-og_num2]=path[num2];
       
    }
   
    int max_j;
    if (file_e->size%512==0){
        max_j=file_e->size/SECTOR_SIZE;}
    else{
        max_j=file_e->size/SECTOR_SIZE+1;}
    
    //READ DISK SECTOR, SAVE TO BUFFER, TEST IF FILE_TEST FILENAME IS EQUAL TO FILE_SEARCHED
    
    for (int j=1; j<max_j+1;j++){

        //fdc_read_sector(disk,3,buf,0,sector_read);
        
        fdc_read_sector(disk,file_e->lba_first-1+j,buf,0,sector_read);
       
        for (int i=0; i<16; i++){//
            
            for (int k=0; k<15; k++){
                file_test->filename[k]=buf[i*32+k];
            }
  
            file_test->is_dir=(int)buf[i*32+15];
            file_test->size=0;
            file_test->lba_first=0;
            file_test->size=(int)buf[i*32+16]<<0 | (int)buf[i*32+17]<<8 | (int)buf[i*32+18]<<16 | (int)buf[i*32+19]<<24;
            file_test->lba_first=(int)buf[i*32+20]<<0 | (int)buf[i*32+21]<<8 | (int)buf[i*32+22]<<16 | (int)buf[i*32+23]<<24;
        

            if(strcmp(file_searched,file_test->filename, 15)==1){
              
                if (strlen(path)<=30){
                
                     for (int k=0; k<15; k++){
                        end_file->filename[k]=buf[i*32+k];
                    }
                    //printf("%endf: %s\n",end_file->filename);
                    end_file->is_dir=(int)buf[i*32+15];
                    end_file->size=0;
                    end_file->lba_first=0;
                    end_file->size=(int)buf[i*32+16]<<0 | (int)buf[i*32+17]<<8 | (int)buf[i*32+18]<<16 | (int)buf[i*32+19]<<24;
                    end_file->lba_first=(int)buf[i*32+20]<<0 | (int)buf[i*32+21]<<8 | (int)buf[i*32+22]<<16 | (int)buf[i*32+23]<<24;
                    
                    //printf("size: %d lba: %d\n",end_file->size, end_file->lba_first);
                    //*lba_of_filedir=end_file->lba_first;
                    return true;
                }else{
                    //printf("size: %d lba: %d\n",end_file->size, file_test->lba_first);
                    if (can_change_filedir==true){
                    *lba_of_filedir=file_test->lba_first;}
                    find_file_opo(disk, path_without_file, file_test, file_test, end_file, lba_of_filedir);
                }
                
            }
        }
    }
    return false;
   
}
int read_file_opo(uint32_t disk, file_entry* fe, uint8_t* buf){
    
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
    uint32_t bytes_read=0;
    //printf("sectors to read: %d lba_first: %d  ",sectors_to_read,fe->lba_first);
    int cpy=fe->lba_first;
    
    for (int i=0; i<sectors_to_read; i++){
        
        fdc_read_sector(disk,cpy+i,&temp_buf,0,sector_read);
        
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
int change_file_entry_opo(file_entry* file_e, int new_size, int new_lba){
        unsigned char buf[512];
        memset(buf,0,512);
        fdc_read_sector(0,new_lba,buf,0,sector_read);
       
        unsigned char filename[16];
        filename[15]='\0';
        bool is_dir=0;
        int lba_first=0;
        int size=0;
        unsigned char buf_new[512];
        memset(buf_new,0,512);
        for (int i=0; i<16; i++){//
            
            for (int k=0; k<15; k++){
                filename[k]=buf[i*32+k];
            }
            if (strcmp(filename, file_e->filename, sizeof(file_e->filename))==1){
               
                for (int k=0; k<15; k++){
                   buf_new[i*32+k]=buf[i*32+k];
                }
                
                is_dir=(int)buf[i*32+15];
            

                buf_new[i*32+15]=(int)buf[i*32+15];
                buf_new[i*32+16]=(new_size>>0) & 0xFF;
                buf_new[i*32+17]=(new_size>>8)& 0xFF;
                buf_new[i*32+18]=(new_size>>16)& 0xFF; 
                buf_new[i*32+19]=(new_size>>24)& 0xFF;
                
                size=(int)buf_new[i*32+16]<<0 | (int)buf_new[i*32+17]<<8 | (int)buf_new[i*32+18]<<16 | (int)buf_new[i*32+19]<<24;
                //printf("new_size: %d | size: %d\n",new_size,size);
                buf_new[i*32+20]=buf[i*32+20];
                buf_new[i*32+21]=buf[i*32+21];
                buf_new[i*32+22]=buf[i*32+22];
                buf_new[i*32+23]=buf[i*32+23];
                for (int x=24;x<32;x++){
                    buf_new[i*32+x]=0;
                }
            }
            else{
                for (int x=0; x<32; x++){
                    buf_new[i*32+x]=buf[i*32+x];
                }
            }
            memset(filename,0,16);
            }
            //("|size of file: %d %d %d %d|", (int)buf_new[i*32+16], (int)buf_new[i*32+17] , (int)buf_new[i*32+18], (int)buf_new[i*32+19]);
           
            fdc_write_sector(0,new_lba, buf_new, 0, sector_write);
            return 0;

            //lba_first=(int)buf[i*32+20]<<0 | (int)buf[i*32+21]<<8 | (int)buf[i*32+22]<<16 | (int)buf[i*32+23]<<24;

        
}
int create_file_opofs(file_entry* og_dir, int lba_where_to_create, char* new_filename, int new_lba, int new_size, uint8_t* new_file_data, int dir){

    int sectors_to_read=0;
    int size_mod=og_dir->size%512;
    uint8_t directory_data[512];
    uint8_t new_data[512];
    if (size_mod==0){   
        sectors_to_read=og_dir->size/SECTOR_SIZE;
    }
    else{
        sectors_to_read=(og_dir->size/SECTOR_SIZE)+1;
    }
    bool file_saved=0;
    //printf("filen: %s",new_filename);
    memset(directory_data,0,512);
    memset(new_data,0,512);
    for (int j=0; j<sectors_to_read; j++){
        fdc_read_sector(0,lba_where_to_create+j,&directory_data,0,sector_read);
        for (int i=0; i<16; i++){
            if (directory_data[i*32]!=0){
                for (int z=0; z<32; z++){
                    new_data[i*32+z]=directory_data[i*32+z];
                }
            }
            else{// if nxt ==0
                if (file_saved==0){
                    for (int z=0; z<15; z++){
                        new_data[i*32+z]=new_filename[z];
                    }
                    new_data[i*32+15]=dir;
                    new_data[i*32+16]=(new_size>>0) & 0xFF;
                    new_data[i*32+17]=(new_size>>8)& 0xFF;
                    new_data[i*32+18]=(new_size>>16)& 0xFF; 
                    new_data[i*32+19]=(new_size>>24)& 0xFF;
                    
                    //size=(int)buf_new[i*32+16]<<0 | (int)buf_new[i*32+17]<<8 | (int)buf_new[i*32+18]<<16 | (int)buf_new[i*32+19]<<24;
                    //printf("new_size: %d | size: %d\n",new_size,size);
                    new_data[i*32+20]=new_lba>>0;
                    new_data[i*32+21]=new_lba>>8;
                    new_data[i*32+22]=new_lba>>16;
                    new_data[i*32+23]=new_lba>>24;
                    for (int z=24; z<32; z++){
                        new_data[i*32+z]=0;
                    }
                    file_saved=1;
                }
            }
        }
        
        if (file_saved==1){
            if (new_size%512==0){
                allocate_sectors(new_lba, (new_size%512));
            }
            else{
                allocate_sectors(new_lba, (new_size%512)+1);
            }
           
            fdc_write_sector(0,lba_where_to_create+j,new_data,0,sector_write);
            
            memset(&directory_data,0,512);
            memset(&new_data,0,512);
            
            return 1;
        }else{
             fdc_write_sector(0,lba_where_to_create+j,new_data,0,sector_write);
            memset(&directory_data,0,512);
            memset(&new_data,0,512);
        }
    }
    return 0;
}

void  opo_path_formatter(char* og_path, char* new_path, int count_elements){
   
    int add_spaces=0;
    int iter=0;
    int path_el_length=0;
    int was_dot=0;
    int spaces_to_add_ext=0;
    int already_added_dot=0;

    for (int i=0; i<count_elements+1; i++){
        for (int j=0; j<15; j++){
            
            if (og_path[iter]=='.' ){
                add_spaces=1;
                spaces_to_add_ext=15-path_el_length-3;//for example3    txt| 
                new_path[(i*16)+j]=' ';
                already_added_dot++;
              
                if (already_added_dot==spaces_to_add_ext){
                    iter++;
                    add_spaces=0;
                    continue;
                }
            }
             if (og_path[iter]=='/' || og_path[iter]==0){
                add_spaces=1;
                new_path[(i*16)+j]=' ';
            }
            if (add_spaces==1){
                
                new_path[(i*16)+j]=' ';
            }
             else{
                //printf("%c ", og_path[iter]);
                new_path[(i*16)+j]=og_path[iter];
                iter++;
                path_el_length++;
            }
        }
        new_path[(i*16)+15]='/';
        iter++;
        path_el_length=0;
        add_spaces=0;
        
    }

    //new_path[(count_elements*15)+15]=' ';
    new_path[(count_elements*16)+15]='\0';
   
}
//opofs_move_file_and_create_space_for_size() TODO


int setup_global_file_info_table(){
    uint8_t global_filesystem_data[512];
    int global_filesystem_iter=0;
    memset(global_filesystem_data,0,512);
    uint8_t buf[512];
    uint8_t bits=0;
    int bits_iter=0;
    for (int i=0; i<152; i++){
        
        
        bits = bits | (1<<7-bits_iter);
        bits_iter++;
        if (bits_iter==8){
            global_filesystem_data[global_filesystem_iter]=bits;
            global_filesystem_iter++;
            bits_iter=0;
            bits=0;
        }
    }
    //global_filesystem_data[1]=170;
   
    fdc_write_sector(0,1,global_filesystem_data,0,sector_write);
    return 1;
}

int getbit(uint8_t* bits, int bit){
    uint8_t score=(*bits & 1<<(bit));
    return score;
}
int setbit(uint8_t* bits, int bit, int val){
    uint8_t score=(*bits ^ 1<<(bit));    
    *bits=score;
    
}
int find_free_sectors_in_disk(int size){
    int sectors_to_fit;
    if (size%512==0){   
        sectors_to_fit=size/SECTOR_SIZE;
    }
    else{
        sectors_to_fit=(size/SECTOR_SIZE)+1;
    }
    if (sectors_to_fit==0){
        sectors_to_fit++;
    }
    uint8_t filesystem_info[512];
    fdc_read_sector(0,1,filesystem_info,0,sector_read);
    int bit_sector=-1;
    int free_in_row=0;
    for(int i=0; i<360; i++){
        for (int b=7; b>=0; b--){
            bit_sector=getbit(&filesystem_info[i],b);
            if (bit_sector==0){
                free_in_row++;
            }
            else{
                free_in_row=0;
            }
            if (free_in_row==sectors_to_fit){
                //printf("found: %d",i*8+(7-b));
                return i*8+(7-b);
            }
           
        }
       
    }

}
int allocate_sectors(int lba_start, int sectors){
    uint8_t filesystem_info[512];
    fdc_read_sector(0,1,filesystem_info,0,sector_read);
    int fi_num=lba_start/8;
    int bit=lba_start%8;
    setbit(&filesystem_info[fi_num],7-bit,1);
    fdc_write_sector(0,1,filesystem_info,0,sector_write);
    return 1;
}

int delete_file_or_dir(file_entry* file_to_delete, int parent_lba){
    uint8_t sector_content_read[512];
    int sectors_to_read;
    if (file_to_delete->size%512==0){
        sectors_to_read=file_to_delete->size/512;
    }
    else{
        sectors_to_read=(file_to_delete->size/512)+1;
    }
    if (file_to_delete->is_dir==1){
        for (int i=0; i<sectors_to_read; i++){
            fdc_read_sector(0,file_to_delete->lba_first,sector_content_read,0,sector_read);
            for (int j=0; j<512; j++){
                if (sector_content_read[j]!=0){
                    printf("Couldn't delete directory: not empty.");
                    return 0;
                }
           }
           memset(sector_content_read,0,512);
        }
    }
    memset(sector_content_read,0,512);
    //DELETE HEADER
    uint8_t filename[16];
    memset(filename,0,16);
    filename[15]='\0';
    fdc_read_sector(0,parent_lba,sector_content_read,0,sector_read);
    uint8_t* file_read=mem_allocate(512);
    memset(file_read,0,512);
    for (int b=0; b<16; b++){
        for (int z=0; z<15; z++){
            filename[z]=sector_content_read[b*32+z];
            //file_read[b*32+z]=sector_content_read[b*32+z];
        }
       
        if (strcmp(file_to_delete->filename,filename,15)==1){
            for (int z=0; z<32; z++){
                file_read[b*32+z]=0;
            }
        }
        else{
            for(int z=0; z<32; z++){
                file_read[b*32+z]=sector_content_read[b*32+z];
            }
        }
    }
    fdc_write_sector(0,parent_lba,file_read,0,sector_write);
    memory_free(file_read);

    //DELETE CONTENTS
    uint8_t* file_read_buf=mem_allocate(512);
    memset(file_read_buf,0,512);
    int sector_counter=file_to_delete->lba_first;
    for (int i=0; i<sectors_to_read; i++){
        fdc_write_sector(0,sector_counter,file_read_buf,0,sector_write);
        allocate_sectors(sector_counter,sectors_to_read);
        sector_counter++;
        //setbit in global map
        
    }
   
    memory_free(file_read_buf);

    
    uint8_t temp_buf[512];
    fdc_read_sector(0,1,temp_buf,0,sector_read);
   
    //printf("Deleted: %s\n", file_to_delete->filename);
    return 1;
    
}




int only_null_until_end(int x, int y){
      for (int column = y; column < 80; column++) {
        if (pte_data[x][column] != 0) {
            return 0;
        }
      }
    return 1;
}
int only_null_until_end_of_file(int x, int y){
    for (int row=x; row<23; row++){
      for (int column = y; column < 80; column++) {
        if (pte_data[row][column] != 0) {
            return 0;
        }
      }
    }
    return 1;
}

int move_to_left(int cur){
    
    uint8_t buf_one;
    uint8_t buf_two;
    
    uint8_t* glob_vid_adr=0xC00B8000+(160*video_y)+(cur-1)*2;
    for (int i=cur; i<strlen(pte_saved_data); i++){
        pte_saved_data[i]=pte_saved_data[i+1];
        //printf("tried moving :%d to %d",i, i-1);
    }
}

void right_arrow_func(){
	    int og_x=video_x;
         int og_y = video_y;
         
         if (video_x == 79) {
           video_y++;
           video_x = 0;
         } else {
           video_x++;
         }
        if (pte_data[video_y-2][video_x]!=0 && pte_data[og_y-2][og_x]!=0){
            update_cursor(video_x, video_y);
            global_cursor++;
            return;
        }
         if ((pte_data[video_y - 2][video_x] == 0 && pte_data[og_y-2][og_x] != 0)) {
            //printf("|nig|");
             update_cursor(video_x, video_y);
            global_cursor++;
            return;
         }
         else{
           while (!(video_y == 24 && video_x == 79)) {
             if (!(pte_data[video_y - 2][video_x] == 0 )) {
                video_x++;
               update_cursor(video_x, video_y);
                //global_cursor++;
               
               break;
             } else {
               if (video_x == 79) {
                 video_y++;
                 video_x = 0;
               } else {
                 video_x++;
               }
               
             }
             
           }
           video_x = og_x;
           video_y = og_y;
           //update_cursor(video_x, video_y);
            if ((video_x!=og_x && video_y!=og_y )){
                global_cursor++;
           }
           
           
         }
        
         
         //printf("glob: %d",global_cursor);
}

void left_arrow_func(){
    if ((pte_mode==1 && pte_cmd_mode==0 )|| (pte_mode==0)){
          if (video_y==2 && video_x==0){
        return;
    }
     int og_x=video_x;
         int og_y = video_y;
      if (video_x == 0) {
        video_y--;
        video_x = 79;
      } else {
        video_x--;
      }
        if (pte_data[video_y-2][video_x]!=0 && pte_data[og_y-2][og_x]!=0){
            update_cursor(video_x,video_y);
            global_cursor--;
            return;
        }
      if ((pte_data[video_y - 2][video_x] == 0 )) {
        while (!(video_y == 2 && video_x == 0)) {
          if (pte_data[video_y - 2][video_x] != 0) {
            //update_cursor(video_x, video_y);
            
            break;
          } else {
            if (video_x == 0) {
              video_y--;
              video_x = 79;
            } else {
              video_x--;
            }
            
          }
         
        }
            video_x = og_x;
           video_y = og_y;
           update_cursor(video_x, video_y);
           if (video_x!=og_x && video_y!=og_y){
                global_cursor--;
           }
           
      }else{
        //global_cursor--;
          
        update_cursor(video_x, video_y);
      }
    
    }
}


void up_arrow(){
      if ((pte_mode==1 && pte_cmd_mode==0 )|| (pte_mode==0 && pte_cmd_mode==0)){
                        if(video_y==2){
                            if (top_cursor==0 && video_y==2){
                               return;
                            }
                            top_cursor--;
                            refresh_based_on_pte_global(top_cursor);
                            video_y=2;
                            video_x=0;
                            update_cursor(video_x, video_y);
                            return;
                        }
                       
                       //video_y--;
                       int save_vid_x=video_x;
                       int save_vid_y=video_y;
                       while(!(video_y==save_vid_y-1 && save_vid_x==video_x)){
                            if (video_x==0){
                                video_x=79;
                                video_y--;
                            }
                            else{
                                video_x--;
                            }
                            if (pte_data[video_y-2][video_x]!=0){
                                global_cursor--;
                                update_cursor(video_x,video_y);
                            }
                       }
                         while (video_x!=0){
                            if (pte_data[video_y-2][video_x]!=0){
                                video_x++;
                                update_cursor(video_x,video_y);
                                break;
                            }
                            video_x--;
                        }
                       //left_arrow_func();
                    
                    }
                    
                    // update_cursor(video_x,video_y);
}
void down_arrow(){
    if ((pte_mode==1 && pte_cmd_mode==0 )|| (pte_mode==0 && pte_cmd_mode==0)){
                        int count=count_file_lines();
                        if (count-22-top_cursor-1<0){
                                return;
                        }
                        if(video_y==24){      
                            top_cursor++;
                            refresh_based_on_pte_global(top_cursor);
                            return;
                        }
                        int save_vid_x=video_x;
                        int save_vid_y=video_y;
                        while(!(video_y==save_vid_y+1 && save_vid_x==video_x)){
                                if (video_x==79){
                                    video_x=0;
                                    video_y++;
                                }
                                else{
                                    video_x++;
                                }
                                if (pte_data[video_y-2][video_x]!=0){
                                    global_cursor++;
                                    update_cursor(video_x,video_y);
                                    
                                }
                        }
                        if (pte_data[video_y-2][video_x]==0 && pte_mode==1){
                            while (video_x!=0){
                                if (pte_data[video_y-2][video_x]!=0){
                                    video_x--;
                                    
                                    update_cursor(video_x,video_y);
                                    break;
                                }
                                video_x--;
                            }
                        }

                        //right_arrow_func();
                        
                    }
                   //update_cursor(video_x,video_y);
}


int return_file_entry(int i, file_entry* save_f, file_entry* file_e){
            
                if (g_cmd_str[i][0]=='/'){
                    //printf("\n\n");
                    int count_elements=0;
                    for (int j=0; j<strlen(g_cmd_str[i])-1;j++){
                        if (g_cmd_str[i][j]=='/'){
                        count_elements++;
                        }
                    }
                    count_elements+=(elements_in_prefix-1);
                    char path[(15*(count_elements+1))+(count_elements+1)];
                    char path_c[strlen(g_cmd_str[i])+strlen(global_cmd_prefix)+1];
                    memset(path,' ',(15*(count_elements+1))+(count_elements+1));
                    memset(path_c,' ',strlen(g_cmd_str[i])+strlen(global_cmd_prefix)+1);

                    for (int j=0; j<strlen(global_cmd_prefix); j++){ //prefix loop
                        path_c[j]=global_cmd_prefix[j];
                    }
                    int iter=0;
                    for (int j=strlen(global_cmd_prefix);j<strlen(path_c);j++){
                        path_c[j]=g_cmd_str[i][iter];
                        iter++;
                    }
                    path_c[strlen(path_c)]='\0';
                    opo_path_formatter(path_c, path, count_elements);
                    
                    int count=0;
                    //printf("%s, %d, %d, %d ",path,root_dir.lba_first,file_e->lba_first, &lba_of_filedir);
                    find_file_opo(0,path,&root_dir,save_f,file_e,&lba_of_filedir);
                     if (strcmp(path,".",1)==1 || strchr(path,'/')==NULL){
                        lba_of_filedir=root_dir.lba_first;
                        count=root_dir.lba_first;
                    }else{
                        
                        count=lba_of_filedir;
                    }
                    
                    //printf("count: %d\n",count);
                    return count;
                    
                }
                else{
               
                    //printf("\n\n");
                    int count_elements=0;
                    for (int j=0; j<strlen(g_cmd_str[i])-1;j++){
                        if (g_cmd_str[i][j]=='/'){
                        count_elements++;
                        }
                    }
                    char path[(15*(count_elements+1))+(count_elements+1)];
                    memset(path,' ',(15*(count_elements+1))+(count_elements+1));
                    
                    opo_path_formatter(g_cmd_str[i], path, count_elements);
                    int count=0;
                    
                    find_file_opo(0,path,&root_dir,save_f,file_e,&lba_of_filedir);
                    
                    count=lba_of_filedir;
                    if (strcmp(path,".",1)==1 || strchr(path,'/')==NULL){
                        lba_of_filedir=root_dir.lba_first;
                        count=root_dir.lba_first;
                    }else{
                        
                        count=lba_of_filedir;
                    }
                    return count;
                    
                }
                
}
int return_file_entry_with_name(char* name, file_entry* save_f, file_entry* file_e){
            
                if (name[0]=='/'){
                    //printf("\n\n");
                    int count_elements=0;
                    for (int j=0; j<strlen(name)-1;j++){
                        if (name[j]=='/'){
                        count_elements++;
                        }
                    }
                    count_elements+=(elements_in_prefix-1);
                    char path[(15*(count_elements+1))+(count_elements+1)];
                    char path_c[strlen(name)+strlen(global_cmd_prefix)+1];
                    memset(path,' ',(15*(count_elements+1))+(count_elements+1));
                    memset(path_c,' ',strlen(name)+strlen(global_cmd_prefix)+1);

                    for (int j=0; j<strlen(global_cmd_prefix); j++){ //prefix loop
                        path_c[j]=global_cmd_prefix[j];
                    }
                    int iter=0;
                    for (int j=strlen(global_cmd_prefix);j<strlen(path_c);j++){
                        path_c[j]=name[iter];
                        iter++;
                    }
                    path_c[strlen(path_c)]='\0';
                    opo_path_formatter(path_c, path, count_elements);
                    
                    int count=0;
                    
                    find_file_opo(0,path,&root_dir,save_f,file_e,&lba_of_filedir)==false;
                    count=lba_of_filedir;
                    return count;
                    
                }
                else{
               
                    //printf("\n\n");
                    int count_elements=0;
                    for (int j=0; j<strlen(name)-1;j++){
                        if (name[j]=='/'){
                        count_elements++;
                        }
                    }
                    char path[(15*(count_elements+1))+(count_elements+1)];
                    memset(path,' ',(15*(count_elements+1))+(count_elements+1));
                    
                    opo_path_formatter(name, path, count_elements);
                    int count=0;
                    find_file_opo(0,path,&root_dir,save_f,file_e,&lba_of_filedir);
                    count=lba_of_filedir;
                    return count;
                    
                }
}


int recognize_command(char* command){
    //printf("\ncommand: %s",command);
    char cmd_buf[strlen(command)];
    memset(cmd_buf,0,strlen(command));
    int cmd_buf_iter=0;
    int cmd_count=0;

    for (int i=0; i<strlen(command);i++){ 
        if (command[i]==' '){
            continue;
        }
        if ((command[i-1]==' ' && command[i]!=' ')){
            
            strcpy(g_cmd_str[cmd_count],cmd_buf);
            memset(&cmd_buf,0,strlen(cmd_buf));
            
            cmd_count++;
            cmd_buf_iter=0; 
        }
        cmd_buf[cmd_buf_iter]=command[i];
        cmd_buf_iter++;
        if (i==strlen(command)-1){
            strcpy(g_cmd_str[cmd_count],cmd_buf);
        }

    }
}
int move_line_down(){
    uint8_t top[80];
    uint8_t bot[80];
    for (int i=2; i<25; i++){
        for (int j=0; j<80; j++){

        }
    }
}



void quit_pte_func(){
    // pte_data[video_y][video_x]='\0';
    pte_mode=0;
    pte_cmd_mode=0;

    return_video_memory();
    update_cursor(save_cursor_x, save_cursor_y);
							//*pte_buf=mem_allocate(1840+25);
    video_x=save_cursor_x; video_y=save_cursor_y;
    int count_bytes=0;
    int count_size=0;
    int sector_to_write=file_opened->lba_first;
    char newbuf[512];
    for (int z=0; z<512; z++){
                                newbuf[z]=0;
                            }
                            if (global_command[1]=='s'){
                                printf("\n");
                                
                                for (int i=0; i<strlen(pte_saved_data)-1; i++){
                                    newbuf[i]=pte_saved_data[i];
                                    count_bytes++;
                                    count_size++;
                                    if (count_bytes==512){
                                        fdc_write_sector(0,sector_to_write,newbuf,0,sector_write);
                                        sector_to_write++;
                                        count_bytes=0;
                                        memset(newbuf,0,512);
                                    }
                                }
                               
                                file_entry save_f;
                                file_entry file_e;
                                fdc_write_sector(0,sector_to_write,newbuf,0,sector_write);
                                
                                int score=return_file_entry_with_name(save_path_for_pte,&save_f,&file_e);
                                
                                change_file_entry_opo(file_opened,count_size,score);
                                
                                memory_free(pte_saved_data);
                                global_cursor=0;
                                top_cursor=0;
                                
                                
    }
}