#pragma once

#include "../../clibs/stdio.h"
#include "../../clibs/stdint.h"
#include "../../clibs/string.h"
#include "fs/opofs.h"
struct file_entry* file_opened;
void pte_escape_handle();
int return_video_memory();
void read_new_contents();
int save_cur_no_cmd_pte_x;
int save_cur_no_cmd_pte_y;
void setup_pte(struct file_entry* file_e);
int save_video_memory();
int8_t saved_mem[2000];
void refresh_based_on_pte_global(int from_line);
int count_file_lines();
int count_line_starting_point();