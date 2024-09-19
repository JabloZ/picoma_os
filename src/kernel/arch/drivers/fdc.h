#include "../../clibs/x86.h"
#include "../../clibs/stdint.h"
#include "../interrupts/pic.h"
#include "../interrupts/irq.h"
#include "../interrupts/isr.h"
#include "../../clibs/stdio.h"
#include "../../memory/page.h"
void reset_fdc();
void fdc_irq_handle();
void fdc_detect_drives();
int fdc_wait_ready();
int fdc_calibrate(int drive);
void fdc_send_command(uint8_t command);

#define FDC_DOR 0x3F2
#define FDC_MSR 0x3F4
#define FDC_FIFO 0x3F5
#define FDC_CCR 0x3F7
#define FDC_READ 0x46
#define FDC_SEEK 0xF
#define DMA_BUFFER 512

int fdc_read_sector(int drive, unsigned cyl);
unsigned char floppy_read_data();

#define FDC_SENSE_INTERRUPT 0x08
#define FDC_RECALIBRATE 0x07

static const char fdc_dma_buffer[0x4800];
