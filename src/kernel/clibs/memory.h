#pragma once
#include "stdint.h"

void* memcpy(void* dest,const void* src, uint16_t dest_size);
void* memset(void* dest, int val, uint16_t dest_size);
int memcmp(const void* ptr1, const void* ptr2, uint16_t size);