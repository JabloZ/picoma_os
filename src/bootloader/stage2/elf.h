#ifndef ELF_H
#define ELF_H
#include "stdint.h"
#include "stdbool.h"
#include "disk.h"
#define ELF_32 1
#define ELF_64 2

#define LITTLE_ENDIAN 1
#define BIG_ENDIAN 2

#define TYPE_RELOCATABLE 1
#define TYPE_EXECUTABLE 2
#define TYPE_SHARED 3
#define TYPE_CORE 4

#define IGNORE_ENTRY 0
#define LOAD_ENTRY 1
#define DYNAMIC_ENTRY 2
#define INTERP_ENTRY 3
#define NOTE_SECTION 4

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

typedef enum{
    NO_SPECIFIC=0,
    SPARC=0x02,
    X86=0x03,
    MIPS=0x08,
    POWERPC=0x14,
    ARM=0x28,
    SUPERH=0x2A,
    IA_64=0x32,
    X86_X64=0x3E,
    AARCH64=0xB7,
    RISC_V=0xF3
} architecture;

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

enum elf_flags{
    elf_exe=0x01,
    elf_writabel=0x02,
    elf_readable=0x04,
};
enum elf_segment_type{
    PT_NULL=0,
    PT_LOAD=1,
    PT_DYNAMIC=2,
    PT_INTERP=3,
    PT_NOTE=4,
    PT_SHLIB=5,
    PT_TLS=7,
    PT_LOOS=0x60000000,
    PT_HIOS=0x6FFFFFFF,
    PT_LOPROC=0x70000000,
    PT_HIPROC=0x7FFFFFFF
};

bool read_elf(DISK* disk, const char* path, void** data_out);
#endif