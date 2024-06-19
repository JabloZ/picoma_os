#include <stdio.h>

typedef struct{
    uint8_t jmp_boot_sector[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t directory_entries;
    uint16_t total_sectors;
    uint8_t media_descryptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t large_sector;

    //extended
    uint8_t drive_number;
    uint8_t _reserved;
    uint8_t signature;
    uint32_t volume_serial_number;
    uint8_t volume_label[11];
    uint8_t system_ident[8];
    uint8_t boot_code[8];
    uint16_t boot_signature;
    


} __attribute__((packed)) bootSector;


int main(int argc, char** argv){
    printf("no no");
    if (argc!=3){
        printf("Wrong syntax. correct usage: %s <disk> <file name>", argv[0])
        return -1;
    }
    
    return 0;
}; 