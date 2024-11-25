#ifndef MEMORY_PARAMS_H
#define MEMORY_PARAMS_H

#include "stdint.h"
typedef struct{
    uint32_t base_low;
    uint32_t base_high;

    uint32_t region_low;
    uint32_t region_high;
    uint32_t region_type;
    uint32_t acpi;
} memory_region;

enum memory_region_type{
    USABLE=1,
    RESERVED=2,
    ACPI_RECLAIMABLE=3,
    ECPI_NVS=4,
    BAD_MEMORY=5
};
typedef struct {
    memory_region* memory_regions;
    uint32_t region_num;
} global_mem_info;
global_mem_info* g_memory_regs;
#endif