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
    
    uint8_t buf[512];
    for (int i=0; i<512; i++){
        buf[i]='z';
    }
    buf[255]='\n';
    buf[127]='\n';
    buf[383]='\n';
    buf[512]='\0';
    FILE* file = fopen("file_entry3.bin", "wb");
    if (file != NULL) {
        
        fwrite(&buf,512,1,file);
        //fwrite(&entry2, sizeof(file_entry), 1, file);
        fclose(file);
        return 0;
    } else {
        return 1;
    }
}
