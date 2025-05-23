#include "elf.h"
saved_context glob_context;

void* read_elf(uint8_t* filename_to_read){
    
    
    //bool find_file_opo(uint32_t disk, char* path, file_entry* file_e, file_entry* file_test, file_entry *end_file, int* lba_of_filedir);
    //int read_file_opo(uint32_t disk, file_entry* fe, uint8_t* buf);
    file_entry f1;
    file_entry f2;
    file_entry save_f;
    file_entry file_e;
    
    return_file_entry_with_name(filename_to_read,&save_f,&file_e);
    uint8_t* buf=mem_allocate(file_e.size);
    memset(buf,0,file_e.size);
    
   
    read_file_opo(0,&file_e,buf);
    //check elf
    
    elf_header* elf_h=(elf_header*)buf;
   
    if (elf_h->magic[0]!=0x7F || elf_h->magic[1]!='E' || elf_h->magic[2]!='L' || elf_h->magic[3]!='F'){
        printf("Not an elf file\n");
        return;
    }
    if (elf_h->endian!=LITTLE_ENDIAN){
        printf("Not little endian.\n");
        return;
    }
    if (elf_h->bits!=ELF_32){
        printf("Not 32 bit ELF.\n");
        return;
    }
    if (elf_h->instruction_set!=0x03){
        printf("not x86 instructions.\n");
        return;
    }
    /*if (elf_h->type!=TYPE_EXECUTABLE){
        printf("%d",elf_h->type);
        printf("not executable.\n");
        return;
    }*/
    
    uint32_t program_header_offset=elf_h->program_header_offset;
    uint32_t program_header_size=elf_h->program_entry_size*elf_h->program_entries_count;
    uint32_t program_entry_size=elf_h->program_entry_size;
    uint32_t program_entries_count=elf_h->program_entries_count;
    program_header* elf_program_headers=(program_header*)(elf_h+program_header_offset);
    
    
    for (int i=0; i<program_entries_count; i++){
        program_header* ph=(program_header*)((uint8_t*)elf_h + program_header_offset + i * program_entry_size);
        
        void* mem=(void*)ph->p_vaddr;
        
        vmm_alloc_page_4kb(kernel_second,mem,ph->p_memsz);
        memset(mem,0,ph->p_memsz);
        memcpy(mem,buf+ph->p_offset,ph->p_filesz);
    }
    
    //return elf_h->entry_offset;
    return (elf_header*)elf_h;
    
    
}
void save_context(uint32_t adr){
    
    __asm__ volatile (
        "mov %%esp, %0\n\t"
        "mov %%ebp, %1\n\t"
        : "=r"(glob_context.esp), "=r"(glob_context.ebp)
        :
        : "memory"
    );
    glob_context.eip = adr;
    
}
void jump_elf(elf_header* elf_h){
    

    typedef void (*entry_point_t)(void);
    entry_point_t entry = (entry_point_t)elf_h->entry_offset;
    entry(); 
    return;
}
void remove_elf(elf_header* elf_h){
    elf_header* hdr = (elf_header*)elf_h;
    
    uint32_t program_header_offset=hdr->program_header_offset;
    uint32_t program_header_size=hdr->program_entry_size*hdr->program_entries_count;
    uint32_t program_entry_size=hdr->program_entry_size;
    uint32_t program_entries_count=hdr->program_entries_count;
    program_header* elf_program_headers=(program_header*)(hdr+program_header_offset);
    
    for (int i=0; i<program_entries_count; i++){
        
        program_header* ph = (program_header*)((uint8_t*)hdr + program_header_offset + i * program_entry_size);

        void* mem=(void*)ph->p_vaddr;
        
        vmm_unmap_page_4kb(kernel_second,mem);
    }
    
    memory_free(hdr);
    print_pmm();
}
