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
void handler_irq_1(){
    uint8_t scancode = inb(0x60);
    char* pressed=get_key_name(scancode);
    if (scancode>0x80){
        return;
    }
    else{
        if (strlen(pressed)==1){
            printf("%s", get_key_name(scancode));
            global_command[global_command_num]=pressed[0];
            global_command_num++;
            
        }
        else{
            switch(scancode){
                case 28: //enter
                    memset(global_command, 0, strlen(global_command));
                    global_command_num=0;
                    printf("\n>");
                    break;
                case 14: //backspace
                    global_command_num--;
                    global_command[global_command_num]=0;
                    remove_char(video_x, video_y);
                    break;
            }


        }
        
        
    }
    
    pic_send_eoi(1);
}