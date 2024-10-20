#include <stdio.h>//gcc -o write_bins write_bins.c && ./write_bins
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
        .filename = "example     txt",
        .is_dir = 0,
        .size = 1100,
        .lba_first = 999,
        .reserved = {0}
    };
    file_entry entry2 = {
        .filename = "testdir        ",
        .is_dir = 1,
        .size = 512,
        .lba_first = 1001,
        .reserved = {0}
    };
    file_entry entry_kernel={
        .filename = "kernel      bin",
        .is_dir = 0,
        .size = 40000,
        .lba_first = 99,
        .reserved = {0}
    };
    FILE* file = fopen("file_entry.bin", "wb");
    if (file != NULL) {
        fwrite(&entry, sizeof(file_entry), 1, file);
        fwrite(&entry2, sizeof(file_entry), 1, file);
        //fwrite(&entry3, sizeof(file_entry), 1, file);
        fwrite(&entry_kernel, sizeof(file_entry), 1, file);
        fclose(file);
        return 0;
    } else {
        return 1;
    }
}
