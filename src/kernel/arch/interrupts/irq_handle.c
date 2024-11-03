#include "irq_handle.h"
const char* get_key_name(int scancode) {
    switch (scancode) {
        case 1: return "ESC";
        case 2: return "1";
        case 3: return "2";
        case 4: return "3";
        case 5: return "4";
        case 6: return "5";
        case 7: return "6";
        case 8: return "7";
        case 9: return "8";
        case 10: return "9";
        case 11: return "0";
        case 12: return "-";
        case 13: return "=";
        case 14: return "Backspace";
        case 15: return "Tab";
        case 16: return "Q";
        case 17: return "W";
        case 18: return "E";
        case 19: return "R";
        case 20: return "T";
        case 21: return "Y";
        case 22: return "U";
        case 23: return "I";
        case 24: return "O";
        case 25: return "P";
        case 26: return "[";
        case 27: return "]";
        case 28: return "Enter";
        case 29: return "CTRL";
        case 30: return "A";
        case 31: return "S";
        case 32: return "D";
        case 33: return "F";
        case 34: return "G";
        case 35: return "H";
        case 36: return "J";
        case 37: return "K";
        case 38: return "L";
        case 39: return ";";
        case 40: return "'";
        case 41: return "`";
        case 42: return "LShift";
        case 43: return "\\";
        case 44: return "Z";
        case 45: return "X";
        case 46: return "C";
        case 47: return "V";
        case 48: return "B";
        case 49: return "N";
        case 50: return "M";
        case 51: return ",";
        case 52: return ".";
        case 53: return "/";
        case 54: return "RShift";
        case 55: return "PrtSc";
        case 56: return "Alt";
        case 57: return "Space";
        case 58: return "Caps Lock";
        case 59: return "F1";
        case 60: return "F2";
        case 61: return "F3";
        case 62: return "F4";
        case 63: return "F5";
        case 64: return "F6";
        case 65: return "F7";
        case 66: return "F8";
        case 67: return "F9";
        case 68: return "F10";
        case 69: return "Num Lock";
        case 70: return "Scroll Lock";
        case 71: return "Home (7)";
        case 72: return "Up (8)";
        case 73: return "PgUp (9)";
        case 74: return "- (NumPad)";
        case 75: return "Left (4)";
        case 76: return "Center (5)";
        case 77: return "Right (6)";
        case 78: return "+ (NumPad)";
        case 79: return "End (1)";
        case 80: return "Down (2)";
        case 81: return "PgDn (3)";
        case 82: return "Ins";
        case 83: return "Del";
        default: return "Unknown";
    }
}
int return_command_num(char* str_cmd){
    
    if (strlen(str_cmd)==0){
        return 0;
    }
    if (strcmp(str_cmd,"la",2)==1){
        return 1;
    }
    if (strcmp(str_cmd,"pf",2)==1){
        return 2;
    }
    if (strcmp(str_cmd,"cd",2)==1){
        return 3;
    }
}
void handler_irq_1(){
    uint8_t scancode = inb(0x60);
    char* pressed=get_key_name(scancode);
    if (scancode>0x80){
        return;
    }
    else{
        if (strlen(pressed)==1){
            if (capslock_pressed==0){
                if ((scancode>=16 && scancode<=25) || (scancode>=30 && scancode<=38) || scancode>=44 && scancode<=50){
                printf("%c", (int)get_key_name(scancode)[0]+32);
                global_command[global_command_num]=(int)pressed[0]+32;}
                else{
                    printf("%c", (int)get_key_name(scancode)[0]);
                    global_command[global_command_num]=(int)pressed[0];
                }
                
            }
            else{
                
                printf("%c", (int)get_key_name(scancode)[0]);
                global_command[global_command_num]=(int)pressed[0];
            } 
        
            global_command_num++;
            
        }
        else{
            switch(scancode){
                case 28: //enter
                    recognize_command(global_command);
                    execute_or_recognize_command();
                    memset(g_cmd_str, 0, sizeof(g_cmd_str));
                    memset(global_command, 0, strlen(global_command));
                    
                    global_command_num=0;
                    printf("\n~/%s>",global_cmd_prefix);
                    break;
                case 14: //backspace
                    if (global_command_num==0){
                        break;
                    }
                    global_command_num--;
                    global_command[global_command_num]=0;
                    remove_char(video_x, video_y);
                    break;
                case 58:
                    if (capslock_pressed==0){
                        capslock_pressed=1;
                    }
                    else{
                        capslock_pressed=0;
                    }
                case 57:
                    printf(" ");
                    global_command[global_command_num]=' ';
                    global_command_num++;
            }


        }
        
        
    }
    
    pic_send_eoi(1);
}






int recognize_command(char* command){
    //printf("\ncommand: %s",command);
    char cmd_buf[strlen(command)];
    memset(&cmd_buf,0,strlen(command));
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
int execute_or_recognize_command(){
    
    for (int i=1; i<10; i++){
        switch(return_command_num(g_cmd_str[i-1])){
            case 0:
                break;
            case 1: {//la 'dir' - list all from dir
                //STRING FORMATTING : ONLY FOR DIRECTORIES FOR NOW 
                if (strlen(g_cmd_str[i])==0){
                    break;
                }
                file_entry file_e;
                file_entry save_f;
                if (g_cmd_str[i][0]=='/'){
                    printf("\n\n");
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
                    find_file_opo(0,path,&root_dir,&save_f,&file_e);
                    
                }
                else{
               
                    printf("\n\n");
                    int count_elements=0;
                    for (int j=0; j<strlen(g_cmd_str[i])-1;j++){
                        if (g_cmd_str[i][j]=='/'){
                        count_elements++;
                        }
                    }
                    char path[(15*(count_elements+1))+(count_elements+1)];
                    memset(path,' ',(15*(count_elements+1))+(count_elements+1));
                    
                    opo_path_formatter(g_cmd_str[i], path, count_elements);
                    find_file_opo(0,path,&root_dir,&save_f,&file_e);
                }
                

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

                uint8_t buf[max_j*512];
                char name[16];
                
                for (int j=0; j<max_j; j++){
                    fdc_read_sector(0,file_e.lba_first+j, &buf,0,sector_read);
                    for (int i=0; i<16; i++){
                        for (int z=0; z<15; z++){
                            name[z]=(char)buf[(i*32)+z];
                        }
                        name[15]='\0';
                        
                        if (name[0]!=0){
                            printf("%s\n",name);
                        }
                        memset(name, 0, 16);
                    }
                }
                
               
                break;} 
            case 2:{ //pf
             
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

                opo_path_formatter(g_cmd_str[i], path, count_elements);
                find_file_opo(0,path,&root_dir,&save_f,&file_e);
                char buf[file_e.size];
                read_file_opo(0,&file_e,&buf);
                for (int c=0; c<file_e.size; c++){
                    printf("%c ",buf[c]);
                }

                break;}
            case 3:{//cd
                char potential_rev[2];
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
            default:
                printf("\nCommand unrecognised!\n");
                break;
        }
    }
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