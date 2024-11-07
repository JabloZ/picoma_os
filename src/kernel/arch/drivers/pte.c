#include "pte.h"
int save_video_memory(){
    int8_t* start_mem=(int8_t*)0xC00B8000;
    for (int i=0; i<SCREEN_HEIGHT; i++){//0 to 25
        for (int j=0; j<SCREEN_WIDTH;j++){//0 to 80
            saved_mem[i*SCREEN_WIDTH+j]=*start_mem; 
            start_mem+=2;
                   
        }
    }
    clear_screen();
}
int return_video_memory(){
    int8_t* start_mem=(int8_t*)0xC00B8000;
    for (int i=0; i<SCREEN_HEIGHT; i++){//0 to 25
        for (int j=0; j<SCREEN_WIDTH;j++){//0 to 80
            *start_mem=saved_mem[i*SCREEN_WIDTH+j];
            start_mem+=2;
                   
        }
    }
    
}
void setup_pte(struct file_entry* file_e){
    save_video_memory();
    printf("FILE: %s |command:",file_e->filename);//komenda 3 litery
    for (int c=0; c<80-32; c++){
        printf(" ");
    }
    for (int c=0; c<80; c++){
        printf("_");
    }
}
void pte_escape_handle(){
    if (pte_cmd_mode==0){
        update_cursor(32,0);
        video_x=32;
        video_y=0;
        pte_cmd_mode=1;
    }
    else{
        update_cursor(2,0);
        video_x=0;
        video_y=2;
        pte_cmd_mode=0;
    }
}