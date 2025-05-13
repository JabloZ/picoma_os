#ifndef ELF_H
#define ELF_H
#include "../clibs/stdint.h"
#include "../clibs/stdio.h"
#include "../clibs/stdbool.h"
#include "../arch/drivers/fs/opofs.h"
#include "kalloc.h"
void read_elf(char* filename);
typedef struct{
    uint8_t magic[4];
    uint8_t bits;
    uint8_t endian;
    uint8_t h_version;
    uint8_t os_abi;
    uint8_t padding[8];
    uint16_t type;
    uint16_t instruction_set;
    uint32_t elf_version;
    uint32_t entry_offset;
    uint32_t program_header_offset;
    uint32_t section_header_offset;
    uint32_t flags;
    uint16_t header_size;
    uint16_t program_entry_size;
    uint16_t program_entries_count;
    uint16_t section_entry_size;
    uint16_t section_entries_count;
    uint16_t section_index;
} __attribute__((packed)) elf_header;

typedef struct{
    uint32_t segment_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t flags;
    uint32_t alignment;
} program_header;

#endif