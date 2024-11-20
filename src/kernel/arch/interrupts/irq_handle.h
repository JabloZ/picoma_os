#pragma once
#include "../drivers/fs/opofs.h"
#include "../../clibs/stdint.h"
#include "../../clibs/stdio.h"
#include "../../clibs/stdbool.h"
#include "../../clibs/string.h"
#include "../../clibs/memory.h"
#include "../drivers/pte.h"
#include "irq.h"

void handler_irq_1();
char g_cmd_str[10][100];
int recognize_command();
int execute_or_recognize_command();
int global_command_num;
char global_command[1024];
char global_cmd_prefix[1024];
int elements_in_prefix;
int pte_mode;
int pte_cmd_mode;
int save_cursor_x;
int save_cursor_y;
uint8_t save_path_for_pte[1024];

int global_page_opened_file;
int char_count;
char pte_data[23][80];
int lba_of_filedir;
int return_command_num(char* str);

void left_arrow_func();
void right_arrow_func();
uint8_t* pte_saved_data;
uint32_t global_cursor;
uint32_t top_cursor;
uint32_t size_of_file;