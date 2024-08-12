#include "elf.h"
#include "fat.h"
#include "memdef.h"
#include "memory.h"
#include "stdio.h"
#include "disk.h"
bool read_elf(DISK* disk, const char* path, void** data_out){
    uint8_t* header_load=ELF_LOAD_ADDR;
    uint8_t* elf_load_addr=MEMORY_LOAD_KERNEL;
    uint32_t read=0;
    uint32_t file_buf=0;
    
    fat_file* file_read=open_fat(disk, path);
  
    if((read=read_fat_file(disk, file_read, sizeof(elf_header), header_load))!=sizeof(elf_header)){ 
        printf("\nnot elf, error!");
        return false;
    }
    
    
    file_buf+=read;
    elf_header* header=(elf_header*)header_load;
  
    if (header->magic[0]!= 0x7F || header->magic[1]!= 'E' || header->magic[2]!= 'L' || header->magic[3]!= 'F' ){
        printf("\nnot elf (magic)");
        return false;
    }
    if (header->bits!=ELF_32){
        printf("\nnot 32bit");
        return false;
    }
    if (header->instruction_set!=X86){
        printf("\n not x86 instructions");
        return false;
    }
    if (header->endian!=LITTLE_ENDIAN){
        printf("\nnot little endain");
        return false;
    }
    
    if (header->type!=TYPE_EXECUTABLE){
        printf("\nnot exe");
        return false;
    }
    
    *data_out=(void*)header->entry_offset;
    uint32_t program_header_size=(header->program_entry_size)*(header->program_entries_count);
    //cos sie zjebalo tutaj bo sie entry zeruje
    file_buf+=read_fat_file(disk, file_read, header->program_header_offset-file_buf, header_load);

   
    header_load+=sizeof(elf_header);
    if ((read=read_fat_file(disk, file_read, program_header_size, header_load))!=program_header_size){

        printf("\nnot elf, error!");
        return false;
    }

    file_buf+=read;
    close_file(file_read);
    
    for (uint32_t i=0; i<header->program_entries_count; i++){
        program_header* prog_h=(program_header*)(header_load+i*header->program_entry_size);
        if (prog_h->segment_type==PT_LOAD){
            //read header
            uint8_t* load_buf=(uint8_t*)prog_h->p_vaddr;
            memset(load_buf, 0, prog_h->p_memsz);

            file_read=open_fat(disk, path);
            while(prog_h->p_offset>0){
                uint32_t how_much_to_read=prog_h->p_offset;
                if(prog_h->p_offset>ELF_LOAD_SIZE){
                    how_much_to_read=ELF_LOAD_SIZE;
                }
                read=read_fat_file(disk, file_read, how_much_to_read, elf_load_addr);
                if (read!=how_much_to_read){
                    printf("wrong amount of bytes");
                    return false;
                }
                prog_h->p_offset-=read;
            }
            //read program
            while(prog_h->p_filesz>0){
               
                uint32_t how_much_to_read=prog_h->p_filesz;
                if(prog_h->p_filesz>ELF_LOAD_SIZE){
                    how_much_to_read=ELF_LOAD_SIZE;
                }
                read=read_fat_file(disk, file_read, how_much_to_read, elf_load_addr);
                if (read!=how_much_to_read){
                    printf("wrong amount of bytes");
                    return false;
                }

                prog_h->p_filesz-=how_much_to_read;
                memcpy(load_buf, elf_load_addr, read);
                load_buf+=read;

            }
            close_file(file_read);
        }
    }
    
   
    return true;
}
