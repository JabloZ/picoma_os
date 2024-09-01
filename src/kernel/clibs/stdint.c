#include "stdint.h"
void* P2V(void* physical_address) {
    return (void*)((unsigned int)physical_address + VIRTUAL_BASE);
}