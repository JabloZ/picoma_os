#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t filename[15];
    uint8_t is_dir;
    uint32_t size;
    uint32_t lba_first;
    uint8_t reserved[8];
} __attribute__((packed)) file_entry;

int main() {
    file_entry entry = {
        .filename = "example.txt",
        .is_dir = 0,
        .size = 123456789,
        .lba_first = 100,
        .reserved = {0}
    };

    FILE* file = fopen("file_entry.bin", "wb");
    if (file != NULL) {
        fwrite(&entry, sizeof(file_entry), 1, file);
        fclose(file);
        return 0;
    } else {
        return 1;
    }
}
