#pragma once
#include "stdint.h"

const char* strchr(const char* str, char chr);
char* strcpy(char* destination, char* source);
unsigned strlen(const char* str);
int strcmp(const char* str1, const char* str2, int len);
unsigned strlen_not_space(const char* str);