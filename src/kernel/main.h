#ifndef MAIN_H
#define MAIN_H
#include "clibs/stdint.h"
#include "clibs/stdio.h"
#include "clibs/memory.h"
#include "clibs/x86.h"
#include "arch/init_services.h"
#include "arch/interrupts/irq.h"
#include "arch/interrupts/idt.h"
#include "arch/disk.h"
#include "arch/drivers/fs/opofs.h"
#include "arch/drivers/fdc.h"
#include "arch/drivers/fs/fat.h"
#include "memory_params.h"
#include "memory/page.h"
#include "memory/kalloc.h"
#include "memory/pmm.h"
#include "elf.h"
#include "proc/tss.h"

#endif