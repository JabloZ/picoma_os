#ifndef GDT_H
#define GDT_H
#include "../../clibs/stdint.h"
#define GDT_CODE_SEGMENT 0x08
#define GDT_DATA_SEGMENT 0x10

void init_gdt();

typedef struct{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry;

typedef struct{
    uint16_t limit;
    uint32_t gdt_ptr;
} __attribute__((packed)) gdt_desc;

gdt_desc g_gdt_desc;
int baba;
__attribute__((aligned(2048))) gdt_entry g_gdt_entries[2048];
void __attribute__((cdecl)) load_gdt(gdt_desc* descriptor, uint16_t code_seg, uint16_t data_seg);
#endif
