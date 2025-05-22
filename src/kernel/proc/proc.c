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
    void* user_stack = (void*)0xBFFFE000;

    void* stack_phys=pmm_alloc();
    void* stack=vmm_alloc_page_4kb(kernel_second,user_stack,stack_phys);
    main_thread->initial_stack=user_stack;
    main_thread->frame.esp=main_thread->initial_stack;
    main_thread->frame.ebp=main_thread->frame.esp;
    main_thread->frame.eip=entry;
    switch_current_dir((uint32_t)VIRT_TO_PHYS(&page_directory[767]));
    jump_to_process(main_thread->frame.eip,main_thread->frame.esp);
    
    //entry(); 
}
