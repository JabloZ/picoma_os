#ifndef FDC_H
#define FDC_H
#define FDC_DMA_ADDR ((uint8_t*)0x10000)
#include "../../clibs/x86.h"
#include "../../clibs/stdint.h"
#include "../interrupts/pic.h"
#include "../interrupts/irq.h"
#include "../interrupts/isr.h"
#include "../../clibs/stdio.h"
#include "../../memory/page.h"

#define sector_read 1
#define sector_write 2
int received_int;
void fdc_reset();
void init_fdc();
void fdc_irq_handle();
void fdc_detect_drives();
int fdc_wait_ready();
void init_dma();
int fdc_calibrate(int drive);
void fdc_send_command(unsigned char command);
int fdc_read_sector(int drive, int lba, uint8_t* data_out, int how_many_until, unsigned char cmd);
int fdc_write_sector(int drive, int lba, uint8_t* data_out, int how_many_until, unsigned char cmd);
int fdc_read_sectors(int drive, int lba, int sectors_to_read, uint8_t* data_out, int cmd);
int lba_to_chs(int lba, uint16_t* cyl, uint16_t* head, uint16_t* sec);
void dma_floppy_read();
void dma_floppy_write();
uint8_t floppy_wait_irq();
#define SECTOR_SIZE 512
#define FDC_DOR 0x3F2
#define FDC_MSR 0x3F4
#define FDC_FIFO 0x3F5
#define FDC_CCR 0x3F7
#define FDC_READ 0x46
#define FDC_SEEK 0x0F
#define DMA_BUFFER 512

int fdc_read_track(int drive, unsigned cyl);
unsigned char floppy_read_data();

#define FDC_SENSE_INTERRUPT 0x08
#define FDC_RECALIBRATE 0x07

void floppy_irq();
#define floppy_dma_len 0x200
uint8_t fdc_dma_buffer[512];
static uint8_t fdc_sector_buffer[0x200];
#endif