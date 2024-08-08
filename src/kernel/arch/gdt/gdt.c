#include "../../clibs/stdint.h"
#include "gdt.h"

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

typedef enum{
    //code
    GDT_A_CODE_READABLE = 0x02,
    GDT_A_CODE_PRIVILIGE_EXE=0x04,
    GDT_A_CODE_SEGMENT=0x18,

    //data
    GDT_A_DATA_NORMAL=0x0,
    GDT_A_DATA_WRITEABLE = 0x02,
    GDT_A_DATA_DOWN=0x04,
    GDT_A_DATA_SEGMENT=0x10,
    
    //privilige
    GDT_A_RING_0=0x0,
    GDT_A_RING_1=0x20,
    GDT_A_RING_2=0x40,
    GDT_A_RING_3=0x60,

    //DIF
    GDT_A_TSS=0x0,
    GDT_A_PRESENT=0x80
} GDT_ACCESS;

typedef enum{
    GDT_FLAG_GRANULARITY_1BYTE=0x00,
    GDT_FLAG_GRANULARITY_4KB=0x80,

    GDT_FLAG_32_BIT = 0x40,
    GDT_FLAG_16_BIT = 0x00
} GDT_FLAG;

#define GDT_ENTRY(base,limit,access,flags){\  
    (limit & 0xFFFF), \
    (base & 0xFFFF), \
    ((base >> 16) & 0XFF), \
    access, \
    (((limit >> 16) & 0xF) | (flags & 0xF0)), \
    ((base >> 24) & 0xFF) \
}

void __attribute__((cdecl)) load_gdt(gdt_desc* descriptor, uint16_t code_seg, uint16_t data_seg);

void init_gdt(){
    __attribute__((aligned(2048))) gdt_entry g_gdt_entries[]={
    GDT_ENTRY(0,0,0,0),
    GDT_ENTRY(0,0x000FFFFF,GDT_A_PRESENT | GDT_A_RING_0 | GDT_A_CODE_SEGMENT | GDT_A_CODE_READABLE, GDT_FLAG_32_BIT | GDT_FLAG_GRANULARITY_4KB),
    GDT_ENTRY(0,0x000FFFFF,GDT_A_PRESENT | GDT_A_RING_0 | GDT_A_DATA_SEGMENT | GDT_A_DATA_WRITEABLE, GDT_FLAG_32_BIT | GDT_FLAG_GRANULARITY_4KB),
    
    };
    gdt_desc g_gdt_desc;
    g_gdt_desc.limit=sizeof(g_gdt_entries)-1;
    g_gdt_desc.gdt_ptr=(uint32_t)&g_gdt_entries;


    load_gdt(&g_gdt_desc, GDT_CODE_SEGMENT, GDT_DATA_SEGMENT);
    
}
