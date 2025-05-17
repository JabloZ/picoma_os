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
    if (strcmp(str_cmd,"pte",3)==1){
        return 4;
    }
    if (strcmp(str_cmd,"mkf",3)==1){
        return 5;
    }
    if (strcmp(str_cmd,"mkd",3)==1){
        return 6;
    }
    if (strcmp(str_cmd,"rmv",3)==1){
        return 7;
    }
    if (strcmp(str_cmd,"help",4)==1){
        return 8;
    }
}
void handler_irq_1(){
    
    uint8_t scancode = inb(0x60);
    
    char* pressed=get_key_name(scancode);
    if (scancode>0x80){
		
		switch(scancode){
			case 0xAA: //shift unpressed
			
			shift_pressed=0;
			break;
		}
        pic_send_eoi(1);
        return;
    }

    else{
		switch(scancode){
		case 2://
			if (shift_pressed==1){
				pressed[0]='!';
			}
			else{
				pressed[0]='1';
			}
			break;
		case 3://
			if (shift_pressed==1){
				pressed[0]='@';
			}else{
				pressed[0]='2';
			}
			break;
		case 4://
			if (shift_pressed==1){
				pressed[0]='#';
			}else{
				pressed[0]='3';
			}
			break;
		case 5://
			if (shift_pressed==1){
				pressed[0]='$';
			}else{
				pressed[0]='4';
			}
			break;
		case 6://
			if (shift_pressed==1){
				pressed[0]='%';
			}else{
				pressed[0]='5';
			}
			break;
		case 7://
			if (shift_pressed==1){
				pressed[0]='^';
			}else{
				pressed[0]='6';
			}
			break;
		case 8://
			if (shift_pressed==1){
				pressed[0]='&';
			}else{
				pressed[0]='7';
			}
			break;
		case 9://
			if (shift_pressed==1){
				pressed[0]='*';
			}else{
				pressed[0]='8';
			}
			break;
		case 10://
			if (shift_pressed==1){
				pressed[0]='(';
			}else{
				pressed[0]='9';
			}
			break;
		case 11://
			if (shift_pressed==1){
				pressed[0]=')';
			}else{
				pressed[0]='0';
			}
			break;
		case 12://
			if (shift_pressed==1){
				pressed[0]='-';
			}else{
				pressed[0]='_';
			}
			break;
		case 13://
			if (shift_pressed==1){
				pressed[0]='+';
			}
			else{
				pressed[0]='=';
			}
			break;
		
		case 26://[
			if (shift_pressed==1){
				pressed[0]='{';
			}else{
				pressed[0]='[';
			}
			break;
		case 27://]
			if (shift_pressed==1){
				pressed[0]='}';
			}else{
				pressed[0]=']';
			}
			break;
		case 39://]
			if (shift_pressed==1){
				pressed[0]=':';
			}else{
				pressed[0]=';';
			}
			break;
		case 40://]
			if (shift_pressed==1){
				pressed[0]='"';
			}else{
				pressed[0]=39;//'
			}
			break;
		case 43://]
			if (shift_pressed==1){
				pressed[0]='|';
			}else{
				pressed[0]=92;
			}
			break;
		case 53://]
			if (shift_pressed==1){
				pressed[0]='?';
			}else{
				pressed[0]='/';
			}
			break;
		}


        if (strlen(pressed)==1){
            
            if (capslock_pressed==0){ //capslock not pressed
                if (((scancode>=16 && scancode<=25) || (scancode>=30 && scancode<=38) || scancode>=44 && scancode<=50)){//basically - is alnum
                     printf("%c", (int)get_key_name(scancode)[0]+32);
                    if ((pte_cmd_mode==1 && pte_mode==1)||(pte_mode==0 && pte_cmd_mode==0)){
                        global_command[global_command_num]=(int)pressed[0]+32;
                        global_command_num++;
                        
                    }
                    else if (pte_cmd_mode==0 && pte_mode==1){
                         pte_saved_data[global_cursor]=(int)pressed[0]+32;
                        global_cursor++;
                        pte_data[video_y-2][video_x]=(int)pressed[0]+32;
                        char_count++;
                    }
                }
                else{
                    printf("%c", (int)get_key_name(scancode)[0]);
                    if ((pte_cmd_mode==1 && pte_mode==1) || (pte_mode==0 && pte_cmd_mode==0)){
                    global_command[global_command_num]=(int)pressed[0];
                    global_command_num++;
					}
					else if (pte_cmd_mode==0 && pte_mode==1){
                         pte_saved_data[global_cursor]=(int)pressed[0];
                        global_cursor++;
                        pte_data[video_y-2][video_x]=(int)pressed[0];
                        char_count++;
                    }
                    
                }
                
            }


            else{//capslock pressed
                printf("%c", (int)get_key_name(scancode)[0]);
                if ((pte_cmd_mode==1 && pte_mode==1) || (pte_mode==0 && pte_cmd_mode==0)){
                    global_command[global_command_num]=(int)pressed[0];
                    global_command_num++;
                }
                else if (pte_cmd_mode==0 && pte_mode==1){
                    pte_data[video_y-2][video_x]=(int)pressed[0]+32;
                    pte_saved_data[global_cursor]=(int)pressed[0]+32;
                    global_cursor++;
                    char_count++;
                }
                
            } 
        }
        else{
            switch(scancode){
				case 42:
					shift_pressed=1;
					break;
				case 28: //enter
                    
                    if (pte_mode==1 && pte_cmd_mode==1){
                       
                        if (global_command[0]=='q'){
                            pic_send_eoi(1);
                           quit_pte_func();
                           break;
                        }
                        else{
                            video_x=32;
                            video_y=0;
                            update_cursor(32,0);
                            int8_t* video_memory_pte = (int8_t *) 0xC00B8000+64;
                            for (int i=0; i<48; i++){
                                *video_memory_pte=' ';
                                video_memory_pte+=2;
                            }
                        }
                    }
                    if (pte_cmd_mode==0 && pte_mode==1){
                        
                        if (video_y==24){
                            
                            top_cursor++;
                            refresh_based_on_pte_global(top_cursor);
                            pte_saved_data[global_cursor]='\n';
                            global_cursor++;
                            video_x=0;
                            video_y=24;
                            update_cursor(video_x,video_y);
                            break;
                        }else{
                            
                            pte_saved_data[global_cursor]='\n';
                            global_cursor++;
                            video_y++;
                            video_x=0;
                            update_cursor(video_x, video_y);
                        break;
                        }
                       
                        
                    }
                  
                    recognize_command(global_command);
                    pic_send_eoi(1);
                    
                    execute_or_recognize_command();
                    memset(g_cmd_str, 0, 1000);
                    memset(global_command, 0, 1024);
                    global_command_num=0;
                    if (pte_mode==0){
                        printf("\n~/%s>",global_cmd_prefix);
                    }
                    break;
                case 14: //backspace
                    if (global_command_num==0 && pte_mode==0){
                        break;
                    }
                    if (pte_cmd_mode==0 && pte_mode==1){
                        if (video_y==2 && video_x==0){
                            
                            break;
                        }
                        if (video_x==0){
                               if(video_y==2){
                                if (top_cursor==0 && video_y==2){
                                    break;
                                }   
                                top_cursor--;
                                refresh_based_on_pte_global(top_cursor);
                                video_y=2;
                                video_x=0;
                                update_cursor(video_x, video_y);
                                break;
                                }
                            
                            //video_y--;
                                int save_vid_x=video_x;
                                int save_vid_y=video_y;
                                video_x=79;
                                video_y--;
                                while(video_x!=1){
                                    if (pte_data[video_y-2][video_x-1]!=0){
                                        move_to_left(global_cursor);
                                        global_cursor--;
                                        update_cursor(video_x,video_y);
                                        break;
                                    }
                                    video_x--;
                                }
                                if (video_x==0){
                                    move_to_left(global_cursor);
                                    global_cursor--;
                                    update_cursor(video_x,video_y);
                                    break;
                                }
                        

                        break;
                        }
                        else{
                            int og_y=video_y;
                            while (!(video_y==2 && video_x==0)){
                                if (pte_data[video_y-2][video_x-1]!=0){
                                    
									if (og_y==video_y){
                                        //pte_data[video_y-2][video_x]=0;
                                       
                                        //pte_saved_data[global_cursor-1]=0;
                                        
                                        global_cursor--;
                                        move_to_left(global_cursor);
                                        //global_cursor--;
                                        int save_vid_x=video_x;
                                        int save_vid_y=video_y;
                                        refresh_based_on_pte_global(top_cursor);
                                        video_x=save_vid_x-1;
                                        video_y=save_vid_y;
                                        update_cursor(video_x,video_y);
                                        break;
                                    }
                                    //update_cursor(video_x, video_y);
                                    //pte_saved_data[global_cursor-1]=0;
                                        global_cursor--;
                                        move_to_left(global_cursor);
                                        //global_cursor--;
                                        int save_vid_x=video_x;
                                        int save_vid_y=video_y;
                                        refresh_based_on_pte_global(top_cursor);
                                        video_x=save_vid_x-1;
                                        video_y=save_vid_y;
                                        update_cursor(video_x,video_y);
                                    break;
                                }
                                else{ 
                                    //global_cursor--;
                                   
                                    //remove_char(video_x,video_y);             
                                }
                                break;
                            }
                            
                                        

                        }
                        break;
                    }
                    if (pte_cmd_mode==1 && pte_mode==1){
                        if (video_x==32 && video_y==0){
                            break;
                        }
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
                    break;
                case 57:
                    if (pte_mode==1 && pte_cmd_mode==0){
                        pte_saved_data[global_cursor]=' ';
                        global_cursor++;
                        pte_data[video_y-2][video_x]=' ';
                        printf(" ");
                        update_cursor(video_x, video_y);
                    }
                    else{
                        printf(" ");
                    }
                    global_command[global_command_num]=' ';
                    global_command_num++;
                    break;
                case 1:
                    if (pte_mode==1){
                        pte_escape_handle();
                    }
                    
                    break;
                case 0x48: //up arrow
                    
                    up_arrow();
                    break;
                    
                    
                case 0x4B: //left arrow
                
                    left_arrow_func();

                    break;
                case 0x4D: //right arrow
                    
                    right_arrow_func();    
					break;
                case 0x50: //down arrow
                    
                    down_arrow();
                    break;
            }
        }
    }
    
    pic_send_eoi(1);

}
