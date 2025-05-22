#include "proc.h"
uint32_t create_process(char* process_name){

    typedef void (*entry_point_t)(void);
    entry_point_t entry = read_elf(process_name);
    process proc;
    proc.id=1;
    proc.page_directory=kernel_second;
    proc.priority=1;
    proc.state=1;
    proc.thread_count=1;
    
    thread* main_thread;
    main_thread=&proc.threads[0];
    main_thread->kernel_stack=0;
    main_thread->priority=1;
    main_thread->state=1;
    main_thread->initial_stack=0;
    main_thread->stack_limit=(void*) ((uint32_t) main_thread->initial_stack + 4096);
    void* stack_phys=pmm_alloc();
    void* stack=vmm_alloc_page_4kb(kernel_second,entry+0x1000,stack_phys);
    main_thread->initial_stack=stack;
    main_thread->frame.esp=main_thread->initial_stack;
    main_thread->frame.ebp=main_thread->frame.esp;
    main_thread->frame.eip=entry;
    
    jump_to_process(main_thread->frame.eip,main_thread->frame.esp);
    char* msg="afasf";
    __asm__ volatile (
        "mov $0, %%eax\n"     // syscall number: 0 = write
        "mov %0, %%ebx\n"     // arg1 = pointer to string
        "int $0x80\n"
        :
        : "r"(msg)
        : "eax", "ebx"
    );
    //entry(); 
}
