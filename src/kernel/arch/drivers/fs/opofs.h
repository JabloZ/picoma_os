
#include "../../../clibs/stdint.h"

typedef struct
{
    uint32_t size;
    uint32_t lba_first;
    uint8_t filename[15];
    uint8_t is_dir;
    uint8_t reserved[8];
} __attribute__((packed)) file_entry;

