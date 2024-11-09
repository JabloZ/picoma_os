#pragma once

#include "../../clibs/stdio.h"
#include "../../clibs/stdint.h"
#include "../../clibs/string.h"
#include "fs/opofs.h"
void pte_escape_handle();
int return_video_memory();
int save_cur_no_cmd_pte_x;
int save_cur_no_cmd_pte_y;
void setup_pte(struct file_entry* file_e);
int save_video_memory();
int8_t saved_mem[2000];