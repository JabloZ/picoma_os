#include "../../clibs/stdint.h"
#include "../../clibs/stdio.h"
#include "../../clibs/stdbool.h"
#include "../../arch/drivers/fs/opofs.h"
#include "../../clibs/string.h"
#include "../../clibs/memory.h"
#include "irq.h"
void handler_irq_1();
char g_cmd_str[10][100];
int recognize_command();
int execute_or_recognize_command();
int global_command_num;
char global_command[2048];
int return_command_num(char* str);
void opo_path_formatter(char* path, char* path_new, int count_elements);