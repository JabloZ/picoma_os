#include "../clibs/stdbool.h"
#include "../clibs/stdint.h"
#include "../../glibs/memory_params.h"
typedef struct {
    uint32_t* base;
    uint32_t length;
    uint32_t flags;
    vmm_block* next_block;
} vmm_block;
