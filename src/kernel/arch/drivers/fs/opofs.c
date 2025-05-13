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
    bool can_change_filedir=true;
    if (strlen(path)<15){
        return false;
    } 
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
    
    for (num2; num2<strlen(path);num2++){// od 16 do 31, str od 0 do 15
        path_without_file[num2-og_num2]=path[num2];
       
    }
   
    int max_j;
    if (file_e->size%512==0){
        max_j=file_e->size/SECTOR_SIZE;}
    else{
        max_j=file_e->size/SECTOR_SIZE+1;}
    //printf("SZUKANY PLIK: %s SCIEZKA_BEZ_AKTUALNEGO_PLIKU: %s",file_searched, path_without_file);
    //READ DISK SECTOR, SAVE TO BUFFER, TEST IF FILE_TEST FILENAME IS EQUAL TO FILE_SEARCHED
    for (int j=1; j<max_j+1;j++){
        
        fdc_read_sector(disk,file_e->lba_first-1+j,&buf,0,sector_read);
        
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
    printf("sectors to read: %d lba_first: %d  ",sectors_to_read,fe->lba_first);
    int cpy=fe->lba_first;
    
    for (int i=0; i<sectors_to_read; i++){
        
        printf("%d ",i);
        fdc_read_sector(disk,cpy+i,&temp_buf,0,sector_read);
        printf("%d|",i);
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
    //printf("filen: %s",new_filename);
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

void opo_path_formatter(char* og_path, char* new_path, int count_elements){
   
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
opofs_remove_file_and_contents(){

}

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


int execute_or_recognize_command(){
    
    for (int i=1; i<10; i++){
        switch(return_command_num(g_cmd_str[i-1])){
            case 0:
                break;
            case 1: {//la 'dir' - list all from dir
                //STRING FORMATTING : ONLY FOR DIRECTORIES FOR NOW 
                if (strlen_not_space(g_cmd_str[i])==0){
                    printf("\nToo few arguments.\n");
                    break;
                }
                printf("\n\n");
                if (strlen(g_cmd_str[i])==0){
                    break;
                }
                file_entry file_e;
                file_entry save_f;
                
                return_file_entry(i, &save_f, &file_e);
                
                if (strcmp(g_cmd_str[i],".",1)==1){
                    file_e=root_dir;
                }
                else{
                   
                    if (file_e.is_dir==0){
                        printf("[%s] is not a directory",g_cmd_str[i]);
                        break;
                    }
                } 
             
                int max_j=0;
                if (file_e.size%512==0){
                    max_j=file_e.size/512;
                }
                else{
                    max_j=(file_e.size/512)+1;
                }
               
                uint8_t* buf=mem_allocate(max_j*512);
                char name[16];
                for (int j=0; j<max_j; j++){
                  
                    fdc_read_sector(0,file_e.lba_first+j, buf,0,sector_read);
                    for (int x=0; x<16; x++){
                        for (int z=0; z<15; z++){
                            name[z]=(char)buf[(x*32)+z];
                        }
                        name[15]='\0';
                        
                        if (name[0]!=0){
                            uint32_t size=(int)buf[(x*32)+1+15]<<0 | (int)buf[(x*32)+2+15]<<8 | (int)buf[(x*32)+3+15]<<16 | (int)buf[(x*32)+4+15]<<24;
                            uint32_t lba=(int)buf[(x*32)+5+15]<<0 | (int)buf[(x*32)+6+15]<<8 | (int)buf[(x*32)+7+15]<<16 | (int)buf[(x*32)+8+15]<<24;
                            //printf("size1: %d size2: %d size3: %d size4: %d",buf[i*32+1+15]<<0,buf[i*32+2+15],buf[i*32+3+15]<<16,buf[i*32+4+15]<<24);
                            printf("%s | lba: %d | size: %d \n",name,lba,size);
                        }
                        memset(name, 0, 16);
                    }
                }
                
                memory_free(buf);
                
                memset(name,0,16);
                //memset(&file_e, 0, sizeof(file_e));
                //memset(&save_f, 0, sizeof(save_f));
                break;} 
            case 2:{ //pf
                 if (strlen_not_space(g_cmd_str[i])==0){
                    printf("\nToo few arguments.\n");
                    break;
                }

        
                printf("\n");
                int count_elements=0;
                for (int j=0; j<strlen(g_cmd_str[i])-1;j++){
                    if (g_cmd_str[i][j]=='/'){
                    count_elements++;
                    }
                }
                char path[(15*(count_elements+1))+(count_elements+1)];
                memset(path,' ',(15*(count_elements+1))+(count_elements+1));
                
                file_entry file_e;
                file_entry save_f;

                return_file_entry(i, &save_f, &file_e);

                char buf[file_e.size];
                read_file_opo(0,&file_e,&buf);
                for (int c=0; c<file_e.size; c++){
                    
                    printf("%c",buf[c]);
                }
                //printf("pte: %d\n",pte_mode);

                break;}
            case 3:{//cd
                if (strlen_not_space(g_cmd_str[i])==0){
                    printf("\nToo few arguments.\n");
                    break;
                }
                if (g_cmd_str[i][0]=='.' && '.'==g_cmd_str[i][1]){
                    for (int n=strlen(global_cmd_prefix)-1;n>=0; n--){
                        if (global_cmd_prefix[n]=='/'){
                            global_cmd_prefix[n]='\0';
                            break;
                        }
                        global_cmd_prefix[n]='\0';
                    }
                    elements_in_prefix--;
                    break;
                }
                
                int iter_for_path=0;
                int first_strlen_path=strlen(global_cmd_prefix);
                if (elements_in_prefix!=0){
                    global_cmd_prefix[first_strlen_path]='/';
                    first_strlen_path++;
                }
                
                for (int j=0; j<strlen(g_cmd_str[i]); j++){
                    global_cmd_prefix[first_strlen_path]=g_cmd_str[i][j];
                    first_strlen_path++;
                }
               
                elements_in_prefix++;
                break;
            }
            case 4:{ //pte (text editor)
                 if (strlen_not_space(g_cmd_str[i])==0){
                    printf("Too few arguments.\n");
                    break;
                }
               
               top_cursor=0;
                global_cursor=0;
               int lba;
                file_entry save_f;
                file_entry file_e;
                lba=return_file_entry(i,&save_f, &file_e);
                if (strcmp(g_cmd_str[i],".",1)==1){
                    file_e=root_dir;
                }
                char path[16];
                char path_new[16];
                strcpy(path,g_cmd_str[i+1]);
                opo_path_formatter(path, path_new,1);
                //printf("git");
                //printf("file_e size: %d",file_e.size);

                //printf("file e: %s",file_e.filename);
                if (file_e.is_dir==1 || strlen(file_e.filename)!=15){
                    printf("Error: not a file.");
                    break;
                };
                *file_opened=file_e;
                
                pte_mode=1;
                save_cursor_x=video_x;
                save_cursor_y=video_y;
                setup_pte(&file_e);
                
                char_count=file_e.size-1;
                
                unsigned char pte_buf[file_e.size];
              
                read_file_opo(0,&file_e,pte_buf);
                int y_pte=0;
                int x_pte=0;
                //memset(pte_data,0,1840);
                for (int x=0; x<23; x++){
                    for (int y=0; y<80; y++){
                        pte_data[x][y]=0;
                    }
                }
                if (file_e.size<4096){
                    pte_saved_data=mem_allocate(4096);
                    size_of_file=4096;
                }else{
                    pte_saved_data=mem_allocate(file_e.size);
                    size_of_file=((file_e.size/4096)+1)*4096;
                }

                if (file_e.size!=0){

                    for (int c=0; c<file_e.size; c++){
                        pte_saved_data[c]=pte_buf[c];
                        global_cursor++;

                 //remove_char(video_x, video_y);
                    }
                    refresh_based_on_pte_global(0);
                }
                else{
                    video_x=0;
                    video_y=2;
                    update_cursor(video_x,video_y);

                }
                //memory_free(pte_saved_data);
                strcpy(save_path_for_pte,g_cmd_str[i]);
                memset(&file_e, 0, sizeof(file_e));
                memset(&save_f, 0, sizeof(save_f));
                
                strcpy(g_cmd_str[i],"");
                strcpy(g_cmd_str[i-1],"");
                //save_video_memory();
                //clear_screen();
                
                break;
            }
            case 5:{ //mkf
                if (strlen_not_space(g_cmd_str[i])==0){
                    printf("\nToo few arguments.\n");
                    break;
                }
                int lba;
                file_entry save_f;
                file_entry file_e;
                lba=return_file_entry(i,&save_f, &file_e);
                if (strcmp(g_cmd_str[i],".",1)==1){
                    file_e=root_dir;
                }
                char path[16];
                char path_new[16];
                strcpy(path,g_cmd_str[i+1]);
                opo_path_formatter(path, path_new,1);
                
                create_file_opofs(&file_e,file_e.lba_first,path_new,find_free_sectors_in_disk(0),0,0,0);
                break;
            }
            case 6:{ //mkd
                if (strlen_not_space(g_cmd_str[i])==0){
                    printf("\nToo few arguments.\n");
                    break;
                }
                int lba;
                file_entry save_f;
                file_entry file_e;
                lba=return_file_entry(i,&save_f, &file_e);
                if (strcmp(g_cmd_str[i],".",1)==1){
                    file_e=root_dir;
                }
                char path[16];
                char path_new[16];
                strcpy(path,g_cmd_str[i+1]);
                opo_path_formatter(path, path_new,1);
                
                create_file_opofs(&file_e,file_e.lba_first,path_new,find_free_sectors_in_disk(512),512,0,1);
                break;
            }
            case 7:{
                if (strlen_not_space(g_cmd_str[i])==0){
                    printf("\nToo few arguments.\n");
                    break;
                }
                file_entry save_f;
                file_entry file_e;
                int lba=return_file_entry(i,&save_f, &file_e);
                if (strcmp(g_cmd_str[i],".",1)==1){
                    file_e=root_dir;
                }
               
                delete_file_or_dir(&file_e,lba);
                break;
            }
            case 8:
                printf("\n\nHOME DIRECTORY: [.]");
                printf("\n-la [dir] - shows directories and files in path\n-pf [path to file] - prints out file\n-pte [path_to_file] - opens simple text editor\n -click ESCAPE to switch between insert and command mode in pte\n -qs: quit and save, q: don't save\n");
                printf("-cd [dir] - change directory, .. to go back one directory\n");
                printf("-mkf [path] [filename] - create file\n");
                printf("-mkd [path] [directory name] - create directory\n");
                printf("-rmv [path] - delete file/directory\n");
                break;
            default:
                printf("\nCommand unrecognised!\n");
                break;
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