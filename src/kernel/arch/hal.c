#include "hal.h"
#include "idt.h"
#include "gdt.h"

void hal_init(){
    init_gdt();
    init_idt();
}