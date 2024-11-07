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
int return_command_num(char* str);
void opo_path_formatter(char* path, char* path_new, int count_elements);