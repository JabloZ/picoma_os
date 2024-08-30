#include "page.h"
#define PAGE_4MB_SIZE 4*1024*1024
#define PAGE_4KB_SIZE 4*1024
#define KERNEL_SPACE 0xC0000000
page_directory_entry_t kernel_page_directory;
page_directory_entry kernel_dir;
page_table_entry kernel_first[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));
page_table_entry kernel_second[PAGE_TABLE_COUNT] __attribute__((aligned(PAGE_SIZE)));


void __attribute__((section(".prekernel"))) funkcja_sprawdz(){
    int b;
};
void deinitialize(uint32_t virt_addr){
    __asm__ volatile ("invlpg (%0)" : : "r" (virt_addr) : "memory");
}

static void set_current_page_directory(page_directory_entry page_directory)
{
    __asm__ volatile("movl %0, %%eax\n"
                 "movl %%eax, %%cr3":: "r" (page_directory):"eax", "memory");
}

void enable_paging(page_directory_entry *pg){

   __asm__ volatile("mov %0, %%cr3":: "r"(pg));
   uint32_t cr0;
   __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; 
   __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
   
    printf("no error\n");
}


void init_page(){
    
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].user = 0;
    page_directory[0].frame_addr = ((uint32_t)&kernel_first) >> 12;
    
    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        kernel_first[i].present = 1;
        kernel_first[i].read_write = 1;
        kernel_first[i].user = 0;
        kernel_first[i].frame_addr = i;
    }
    uint32_t pd_index = KERNEL_SPACE >> 22;
    page_directory[pd_index].present = 1;
    page_directory[pd_index].read_write = 1;
    page_directory[pd_index].user = 0;
    page_directory[pd_index].frame_addr = ((uint32_t)&kernel_second)>>12;
     for (int j = 0; j < PAGE_TABLE_COUNT; j++) {
        kernel_second[j].present = 1;
        kernel_second[j].read_write = 1;
        kernel_second[j].user = 0;
        kernel_second[j].frame_addr = j;
    }
    
    uint32_t t = KERNEL_SPACE+0x100000;
    uint32_t t2 = 0x100000;
    while (t2 < 0x400000) {
        vmm_map_page_4kb(t, t2);
        t2 += 0x1000;
        t += 0x1000;
        
    }
    set_current_page_directory(page_directory[pd_index-1]);
    enable_paging(&page_directory);
    /*
    
    vmm_map_page_4kb(0x60000000, 0x100000);
    vmm_map_page_4kb(0x600B8000, 0XB8000);
    video_buffer= (int8_t *)0x600B8000;
    video_memory= (int8_t *)0x600B8000;
    //vmm_map_page_4kb(0x6000FFF0, 0x0000FFF0);
    enable_paging(&page_directory);
    uint32_t gdt_ptr[2];
   printf("error \n");
    gdt_ptr[0] = (g_gdt_desc.limit) | (((g_gdt_desc.gdt_ptr +  KERNEL_SPACE) & 0xFFFF) << 16);
    gdt_ptr[1] = ((g_gdt_desc.gdt_ptr +  KERNEL_SPACE) >> 16);
    load_gdt(gdt_ptr, 0x08, 0x10);
    reload_segments();
    
    uint32_t idt_ptr[2];
    idt_ptr[0] = (g_idt_desc.limit) | ((g_idt_desc.base+  KERNEL_SPACE & 0xFFFF) << 16);
    idt_ptr[1] = (g_idt_desc.base +  KERNEL_SPACE >> 16);
    load_idt(idt_ptr);
    printf("error \n");
    //change_stack();
    printf("adr: %p", KERNEL_SPACE + ((uint32_t)_start - 0x100000));
    void (*start_virtual)(void) = KERNEL_SPACE ;
    printf("Jumping to virtual address: 0x%p\n", KERNEL_SPACE);
    printf("Jumping to virtual address: 0x%p\n", start_virtual);
    start_virtual();*/
}


void *vmm_alloc_page_4mb(void *virtual_address) {
    uint32_t physical_address = pmm_alloc(); // Alokuj stronę fizyczną z PMM
    
    if (physical_address != NULL) {
        
        vmm_map_page_4mb(virtual_address, physical_address); // Mapuj ją do wirtualnego adresu
    }
   
    return physical_address;
}
void vmm_map_page_4mb(uint32_t virtual_address, uint32_t physical_address) {

    uint32_t pd_index = ((uint32_t)virtual_address >> 22) ;
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;

   
    page_directory[pd_index].present=1;
    page_directory[pd_index].user=0;
    page_directory[pd_index].read_write=1;
    page_directory[pd_index].frame_addr=physical_address >> 22;
    
    
    printf("kernel_first: %p", physical_address);
}
void vmm_map_page_4kb(uint32_t virtual_address, uint32_t physical_address) {
   
    uint32_t pd_index = ((uint32_t)virtual_address >> 22) ;
    uint32_t pt_index = ((uint32_t)virtual_address >> 12) & 0x3FF;
    if (!page_directory[pd_index].present){
         page_directory[pd_index].present=1;
        page_directory[pd_index].user=0;
        page_directory[pd_index].read_write=1;
        page_directory[pd_index].frame_addr=physical_address >> 22;
    }
    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);

    pt[pt_index].present = 1;
    pt[pt_index].read_write = 1;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = ((uint32_t)physical_address) >> 12;
}




/*
#include "page.h"

#define KERNEL_SPACE 0x60000000
void enable_paging(page_directory_entry *pg){
   __asm__ volatile("mov %0, %%cr3":: "r"(pg));
   uint32_t cr0;
   __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; 
   __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
   
}

extern void change_stack();
extern void reload_segments();
page_table_entry* create_page_table(int i){
    
    page_table_entry* page_table = mem_allocate(sizeof(page_table_entry) * PAGE_TABLE_COUNT);
    page_directory[i].present = 1;
    page_directory[i].read_write = 1;
    page_directory[i].user = 0;
    page_directory[i].frame_addr = ((uint32_t)page_table) >> 12;
    
     for (int j = 0; j < PAGE_TABLE_COUNT; j++) {
        page_table[j].present = 1;
        page_table[j].read_write = 1;
        page_table[j].user = 0;
        page_table[j].frame_addr = j;
    }

    if (page_table == NULL) {
        //rintf("Error: Could not allocate page table.\n");
        return NULL;
    }
    return page_table;
}

void init_page(){
    
    page_directory[0].present = 1;
    page_directory[0].read_write = 1;
    page_directory[0].user = 0;
    page_directory[0].frame_addr = ((uint32_t)&first_page_table) >> 12;
    page_directory[1].present = 1;
    page_directory[1].read_write = 1;
    page_directory[1].user = 0;
    page_directory[1].frame_addr = ((uint32_t)&second_page_table) >> 12;
    
    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        first_page_table[i].present = 1;
        first_page_table[i].read_write = 1;
        first_page_table[i].user = 0;
        first_page_table[i].frame_addr = i;
    }
    for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
        second_page_table[i].present = 1;
        second_page_table[i].read_write = 1;
        second_page_table[i].user = 0;
        second_page_table[i].frame_addr = i+ (0x400000 >> 12);;
    }
    
    uint32_t max_mem= g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].base_low+g_memory_regs_info->memory_regions[g_memory_regs_info->region_num-1].region_low;
    //map_page((void*)0x100000, (void*)0xC0000000);
     //paging now enabled;
    
    //test_paging();
    
    
}


void map_page(void* phys_addr, void* virt_addr) {
    
    uint32_t pd_index = ((uint32_t)virt_addr >> 22) ;
    uint32_t pt_index = ((uint32_t)virt_addr >> 12) & 0x3FF;
    
    if (!page_directory[pd_index].present) {
        
        page_table_entry* page_table = create_page_table(pd_index);
       
        if (!page_table) {
            printf("Failed to allocate page table for pd_index %u.\n", pd_index);
            return;
        }
        page_directory[pd_index].present = 1;
        page_directory[pd_index].read_write = 1;
        page_directory[pd_index].user = 0;
        page_directory[pd_index].frame_addr = ((uint32_t)page_table) >> 12;
    }

    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);
    
    pt[pt_index].present = 1;
    pt[pt_index].read_write = 1;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = ((uint32_t)phys_addr) >> 12;

    //printf("Mapped 0x%p to 0x%p\n", (uint32_t)virt_addr, (uint32_t)phys_addr);
}
void unmap_page(void* virt_addr) {

    uint32_t pd_index = ((uint32_t)virt_addr >> 22); 
    uint32_t pt_index = ((uint32_t)virt_addr >> 12) & 0x3FF;  
    if (!page_directory[pd_index].present) {
        printf("no");
        return;
    }
    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);
    if (pt[pt_index].present) {
        
        pt[pt_index].present = 0;
        pt[pt_index].frame_addr = 0;
    }
    
    
    __asm__ volatile ("invlpg (%0)" : : "r" (virt_addr) : "memory");
    
}
page_table_entry* allocate_page_table() {
    // Alokuj pamięć dla jednej tablicy stron
    page_table_entry* page_table = (page_table_entry*)mem_allocate(sizeof(page_table_entry) * PAGE_TABLE_COUNT);
    if (!page_table) {
        printf("Error: Could not allocate page table.\n");
        return NULL;
    }

    // Wyzeruj wszystkie wpisy w tablicy stron
    memset(page_table, 0, sizeof(page_table_entry) * PAGE_TABLE_COUNT);

    return page_table;
}
void map_single_page(void* phys_addr, void* virt_addr) {
    uint32_t pd_index = ((uint32_t)virt_addr >> 22); 
    uint32_t pt_index = ((uint32_t)virt_addr >> 12) & 0x3FF; 

    if (!page_directory[pd_index].present) {
        page_table_entry* page_table = allocate_page_table();
        if (!page_table) {
            printf("Failed to allocate page table for pd_index %u.\n", pd_index);
            return;
        }

      
        page_directory[pd_index].present = 1;
        page_directory[pd_index].read_write = 1;
        page_directory[pd_index].user = 0;
        page_directory[pd_index].frame_addr = ((uint32_t)page_table) >> 12;
    }

    
    page_table_entry* pt = (page_table_entry*)(page_directory[pd_index].frame_addr << 12);

    pt[pt_index].present = 1;
    pt[pt_index].read_write = 1;
    pt[pt_index].user = 0;
    pt[pt_index].frame_addr = ((uint32_t)phys_addr) >> 12;

    //printf("Mapped virtual address 0x%p to physical address 0x%p\n", virt_addr, phys_addr);
}
void check_paging(){
    for (int i=0; i<0x400000; i+=0x1000){
        
    }
}

void setup_paging(){
    
    
    uint32_t t = KERNEL_SPACE + 0x100000;
    uint32_t t2 = 0x100000;
    map_page(0x100000,KERNEL_SPACE+0x100000);
    map_page(0x500000,KERNEL_SPACE+0x500000);
    while (t2 < 0x800000) {
        map_single_page((void*)t2, (void*)t);
        t2 += 0x1000;
        t += 0x1000;
        
    }
 
    
   
    enable_paging((uint32_t*)&page_directory);
    /*
    uint32_t gdt_ptr[2];
    gdt_ptr[0] = (g_gdt_desc.limit) | (((g_gdt_desc.gdt_ptr +  KERNEL_SPACE) & 0xFFFF) << 16);
    gdt_ptr[1] = ((g_gdt_desc.gdt_ptr +  KERNEL_SPACE) >> 16);
    load_gdt(gdt_ptr, 0x08, 0x10);
    reload_segments();
    
    uint32_t idt_ptr[2];
    idt_ptr[0] = (g_idt_desc.limit) | ((g_idt_desc.base+  KERNEL_SPACE & 0xFFFF) << 16);
    idt_ptr[1] = (g_idt_desc.base +  KERNEL_SPACE >> 16);
    load_idt(idt_ptr);*/
    
    //map_page((void*)0xB8000, (void*)0x600B8000);
    //video_buffer= (int8_t *)0x600B8000;
    //video_memory= (int8_t *)0x600B8000;
    //enable_paging((uint32_t*)&page_directory);
    //change_stack();
    
    
    




