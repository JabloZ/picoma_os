#ifndef PROC_H
#define PROC_H
#include "stdint.h"
#include "../memory/page.h"
typedef struct process{
    uint32_t id;
    uint32_t priority;
    page_directory_entry* page_directory;
    uint32_t state;
    thread threads[5];
}process;
typedef struct thread{
    process* parent;
    void* initial_stack;
    void* stack_limit;
    void* kernel_stack;
    uint32_t priority;
    uint32_t state;
    trap_frame frame;
}thread;
typedef struct trap_frame{
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    uint32_t edi;
    uint32_t esi;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t flags;
}trap_frame;
#endif