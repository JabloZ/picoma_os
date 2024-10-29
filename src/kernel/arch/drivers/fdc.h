#pragma once

#include "../../clibs/x86.h"
#include "../../clibs/stdint.h"
#include "../interrupts/pic.h"
#include "../interrupts/irq.h"
#include "../interrupts/isr.h"
#include "../../clibs/stdio.h"
#include "../../memory/page.h"
#include "fs/opofs.h"
#define sector_read 1
#define sector_write 2

void reset_fdc();
void fdc_irq_handle();
void fdc_detect_drives();
int fdc_wait_ready();
int fdc_calibrate(int drive);
void fdc_send_command(uint8_t command);
int fdc_read_sector(int drive, int lba, uint8_t* data_out, int how_many_until, int cmd);
int fdc_read_sectors(int drive, int lba, int sectors_to_read, uint8_t* data_out, int cmd);
int lba_to_chs(int lba, uint16_t* cyl, uint16_t* head, uint16_t* sec);

#define SECTOR_SIZE 512
#define FDC_DOR 0x3F2
#define FDC_MSR 0x3F4
#define FDC_FIFO 0x3F5
#define FDC_CCR 0x3F7
#define FDC_READ 0x46
#define FDC_SEEK 0xF
#define DMA_BUFFER 512

int fdc_read_track(int drive, unsigned cyl);
unsigned char floppy_read_data();

#define FDC_SENSE_INTERRUPT 0x08
#define FDC_RECALIBRATE 0x07

#define floppy_dma_len 0x4800
static uint8_t fdc_dma_buffer[floppy_dma_len] __attribute__((aligned(0x8000)));
static uint8_t fdc_sector_buffer[0x200];