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
    for (int c=0; c<80-31; c++){
        printf(" ");
    }
    for (int c=0; c<80; c++){
        printf("_");
    }
}
void pte_escape_handle(){
    if (pte_cmd_mode==0){
        save_cur_no_cmd_pte_x=video_x;
        save_cur_no_cmd_pte_y=video_y;
        update_cursor(32,0);
        video_x=32;
        video_y=0;
        pte_cmd_mode=1;
    }
    else{
        update_cursor(save_cur_no_cmd_pte_x,save_cur_no_cmd_pte_y);
        video_x=save_cur_no_cmd_pte_x;
        video_y=save_cur_no_cmd_pte_y;
        pte_cmd_mode=0;
        //printf("")
    }
}
void refresh_based_on_pte_global(int from_line){
    int go_from=count_line_starting_point(from_line);
    //printf("go from buf num: %d",go_from);
    //return;
    for (int i=0; i<23; i++){
        for (int j=0; j<80; j++){
            pte_data[i][j]=0;
        }
    }
    int how_many_to_go=21;
    int count_inline=0;
    int count_chars=0;
    video_x=0;
    video_y=2;
    int vid_x=0;
    int vid_y=0;
    uint8_t* start_video_mem=0xC00B8000+320;
    for (int i=2; i<SCREEN_HEIGHT; i++){//0 to 25
        for (int j=0; j<SCREEN_WIDTH;j++){//0 to 80
            *start_video_mem=' '; 
            start_video_mem+=2;
                   
        }
    }
    while (how_many_to_go>=0){ 
        for (int i=0; i<80; i++){
            if (pte_saved_data[go_from]!='\n'){
                pte_data[video_y-2][video_x]=pte_saved_data[go_from];
                printf("%c",pte_saved_data[go_from]);
                count_inline++;
                go_from++;
                count_chars++;
                if (count_chars>=strlen(pte_saved_data)){
                    return;
                }
            }
            else{
                printf("\n");
                count_chars++;
                how_many_to_go--;
                go_from++;
                break;
            }
        }
        if (count_inline==80){
            how_many_to_go--;
            
        }
        count_inline=0;

    }
}
int count_file_lines(){
    int iter_wanted_line=0;
    int count_if_full_line=0;

      for (int i=0; i<strlen(pte_saved_data); i++){
        for (int x=0; x<80; x++){
            if (pte_saved_data[i]!='\n'){
                i++;
                count_if_full_line++;
            }
            else{
                i=i+1;
                iter_wanted_line++;
                break;
            }
        }
        if (count_if_full_line==80){
            iter_wanted_line++;
            
        }
        count_if_full_line=0;
        
    }
    if (iter_wanted_line==0){
        return 1;
    }
    return iter_wanted_line;
}
int count_line_starting_point(int wanted_line){
    if (wanted_line==0){
        return 0;
    }
    int iter_wanted_line=0;
    int count_if_full_line=0;
  
    for (int i=0; i<strlen(pte_saved_data); i++){
        for (int x=0; x<80; x++){
            if (pte_saved_data[i]!='\n'){
                i++;
                count_if_full_line++;
            }
            else{
                i=i+1;
                iter_wanted_line++;
                break;
            }
        }
        if (count_if_full_line==80){
            iter_wanted_line++;
            
        }
        if (iter_wanted_line==wanted_line){
            return i;
        }
        count_if_full_line=0;
        
    }
}