#pragma once

#include "../../../clibs/stdint.h"
#include "../../../clibs/stdbool.h"
#define SECTOR_SIZE 512
#define ROOT_DIR -1
#define FILES_COUNT 10

//BOOT_SECTOR

typedef struct{
    //bdb
    uint8_t boot_jump[3];
    uint8_t oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t dir_entries;
    uint16_t totaL_sectors;
    uint8_t media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden;
    uint32_t large_sectors;
    //ebr
    uint8_t drive_num;
    uint8_t reserved;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t system_id[8]; 
} __attribute__((packed)) fat_boot_sector;

//
//DIRECTORY ENTRY 
typedef struct{

	uint8_t   filename[11];               //extension (8.3 filename format)
	uint8_t   attrib;                //file attributes
	uint8_t   reserved;
	uint8_t   time_created_ms;         //creation time
	uint16_t  time_created;
	uint16_t  date_created;           //creation date
	uint16_t  date_last_accessed;
	uint16_t  first_cluster_hi_bytes;
	uint16_t  last_mod_time;           //last modification date/time
	uint16_t  last_mod_date;
	uint16_t  first_cluster;          //first cluster of file data
	uint32_t  file_size;              //size in bytes

} __attribute__((packed)) directory_entry;

//FAT FILE MANAGEMENT+INFO
typedef struct{
	uint32_t pos;
	uint32_t file_size;
	bool is_directory;
    int manage;
} fat_file;
typedef struct{
	uint8_t mem[SECTOR_SIZE];
	fat_file file_data;
	uint32_t first_cluster;
	uint32_t current_cluster;
	uint32_t current_sector;
	bool opened;
} file_data;
//
/*
0x01 - Read Only (tylko do odczytu)
0x02 - Hidden (ukryty)
0x04 - System (plik systemowy)
0x08 - Volume Label (etykieta woluminu)
0x10 - Directory (katalog)
0x20 - Archive (archiwalny)
0x0F - Long File Name (długie nazwy plików)
*/
enum fat_atr{
    FAT_READONLY=0x01,
    FAT_HIDDEN=0X02,
    FAT_SYSTEM=0X04,
    FAT_VOLUME=0X08,
    FAT_DIRECTORY=0X10,
    FAT_ARCHIVE=0X20,
    FAT_LONG_FILENAME=0X0F
    
};

typedef struct{
    union{
        fat_boot_sector boot_sector;
        uint8_t boot_sector_bytes[SECTOR_SIZE];
    } BS;
    file_data root_directory;
	file_data opened_files[FILES_COUNT];
} data;
