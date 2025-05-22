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
typedef struct{
	uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
	uint32_t ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	uint32_t esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed)) tss_entry_struct;
#define GDT_ENTRY(base,limit,access,flags){\  
    (limit & 0xFFFF), \
    (base & 0xFFFF), \
    ((base >> 16) & 0XFF), \
    access, \
    (((limit >> 16) & 0xF) | (flags & 0xF0)), \
    ((base >> 24) & 0xFF) \
}
gdt_desc g_gdt_desc;
void test_user_function();
void flush_tss(void);
__attribute__((aligned(2048))) gdt_entry g_gdt_entries[2048];
void __attribute__((cdecl)) load_gdt(gdt_desc* descriptor, uint16_t code_seg, uint16_t data_seg);
#endif
