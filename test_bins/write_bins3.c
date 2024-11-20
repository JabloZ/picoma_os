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
    
    uint8_t buf[1000];
    for (int i=0; i<1000; i++){
        buf[i]='z';
        if (i>1000){
            buf[i]='x';
        }
        if ((i+1)%128==0){
            buf[i]='\n';
        }
    }
    
    FILE* file = fopen("file_entry3.bin", "wb");
    if (file != NULL) {
        
        fwrite(&buf,1000,1,file);
        //fwrite(&entry2, sizeof(file_entry), 1, file);
        fclose(file);
        return 0;
    } else {
        return 1;
    }
}
