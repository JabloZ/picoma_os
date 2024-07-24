#pragma once
#include <stdint.h>
#include <stdbool.h>
void outb(uint16_t port, uint8_t data);

bool x86_reset_disk(uint8_t drive);
bool x86_read_disk(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t sectors_count, void* dataOut);
bool x86_disk_parameters(uint8_t drive, uint8_t* drive_type, uint16_t* cylinders, uint16_t* sectors, uint16_t* heads);