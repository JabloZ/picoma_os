#pragma once

#include "../../clibs/stdio.h"
#include "../../clibs/stdint.h"
#include "../../clibs/string.h"
#include "fs/opofs.h"
void pte_escape_handle();
void setup_pte(struct file_entry* file_e);
int save_video_memory();
int8_t saved_mem[2000];