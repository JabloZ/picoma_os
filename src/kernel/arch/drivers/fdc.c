#include "fdc.h"
static volatile int floppy_motor_state = 0;
#define motor_off 0
#define motor_on 1
#define motor_wait 2
#define type_read 1
#define type_write 2

#define HEAD 2
#define SECTOR 18
#define CYLINDER 80


void init_fdc(){
    fdc_detect_drives();
    g_irq_handler[6]=fdc_irq_handle;
    fdc_reset();
    fdc_calibrate(0);

}
int fdc_control_motor(int state){
    
    if (state==motor_off){ //we want to turn off
        if (floppy_motor_state==0){
            outb(FDC_DOR, 0x1C);
            for (int i=0; i<10000; i++){;};
        }
        floppy_motor_state=motor_on;
    }
    else{
        if (floppy_motor_state==motor_wait){
            //printf("already waiting");
        }
        floppy_motor_state=motor_wait;
    }
}
void fdc_check_interrupt(int* st0, int* cyl){
    fdc_write_command(FDC_SENSE_INTERRUPT);
    *st0=inb(FDC_FIFO);
    *cyl=inb(FDC_FIFO);
}
unsigned long virt_to_phys(void *virt_addr) {
    return ((unsigned long)virt_addr) - 0xC0000000;
}

void init_dma(){
    union{
        unsigned char bytes[4];
        unsigned long l;
    }a, c;
    a.l=virt_to_phys(fdc_dma_buffer);
    c.l=floppy_dma_len-1;

    if((a.l >> 24) || (c.l >> 16) || (((a.l&0xffff)+c.l)>>16)) {
        printf("a.l: %d\n c.l: %d\n combo: %d\n", a.l >> 24, c.l >>16 ,((a.l&0xffff)+c.l)>>16);
        printf("ERROR");
        return 0;
    }
    outb(0x0a, 0x06); //mask dma 2
    outb(0x0c, 0xFF); //reset
    outb(0x04, a.bytes[0]);
    outb(0x04, a.bytes[1]);

    outb(0x81, a.bytes[2]);
    outb(0x0c, 0xFF);
    outb(0x05, c.bytes[0]);
    outb(0x05, c.bytes[1]);
    outb(0x0b, 0x46);
    outb(0x0a, 0x02);
}

int fdc_reset(){
    
    outb(FDC_DOR, 0x0);
    outb(FDC_DOR, 0x1C);
    fdc_control_motor(motor_on);
    
    //one drive
    fdc_write_command(FDC_SENSE_INTERRUPT);
    uint8_t st0=inb(FDC_FIFO);
    uint8_t cylinder=inb(FDC_FIFO);
    outb(FDC_CCR, 0); //1.44 default
   
}

int fdc_calibrate(int drive){
    
    outb(FDC_DOR, 0x1C); //drive 0, reset=1(normal), irq and dma
    fdc_write_command(FDC_RECALIBRATE);
    fdc_write_command(0);
    
    fdc_control_motor(motor_on);
    outb(FDC_DOR, 0x0C);
    fdc_write_command(FDC_SENSE_INTERRUPT);
    inb(FDC_FIFO);
    inb(FDC_FIFO);
    fdc_control_motor(motor_off);
}
int fdc_read_sectors(int drive, int lba, int sectors_to_read, uint8_t* data_out, int cmd){
   
    int heads=2;
    int sectors=18;
    // DELETE THIS IF, IF SUPPORTED BIGGER FAT(32, 16)
    if (lba>=2880){
        printf("LBA OUT OF LIMIT FOR FAT12: MAX IS 2879\n");
        return 0;
    }
    
    int cyl = lba / (heads * sectors);       
    int head = (lba / sectors) % heads;      
    int sector = (lba % sectors) + 1;  
    int new_lba = lba;
    
    int how_many_until=0;
    for (int i=0; i<sectors_to_read; i++){
        if (fdc_read_sector(drive, new_lba, data_out, how_many_until, cmd)!=0){
            return 0;
        };
        new_lba++;
        how_many_until++;
    }
    return 1;

}
int fdc_read_sector(int drive, int lba, uint8_t* data_out, int how_many_until, int cmd){
    int heads=2;
    int sectors=18;
    // DELETE THIS IF, IF SUPPORTED BIGGER FAT(32, 16)
    if (lba>=2880){
        //printf("LBA OUT OF LIMIT FOR FAT12: MAX IS 2879\n");
        return 1;
    }
    int cyl = lba / (heads * sectors);       
    int head = (lba / sectors) % heads;      
    int sector = (lba % sectors) + 1;  
    if (floppy_seek(cyl, 0)){ return -1;};
    if (floppy_seek(cyl, 1)){ return -1;};
    //printf("%d %d %d", cyl,head,sector);
    for (int i=0; i<20; i++){
        init_dma();
        for (int i=0; i<100000; i++){;};
        fdc_write_command(0x46);
        
        fdc_write_command(0);//drive
        fdc_write_command(cyl); 
        fdc_write_command(head);//first head
        fdc_write_command(1);//first sector
        fdc_write_command(2);//bytes per sector, but counted differently
        fdc_write_command(18);//tracks
        fdc_write_command(0x1b); //gap3
        fdc_write_command(0xff); //data len
        
        unsigned char st0, st1, st2, rcy, rhe, rse, bps;
        st0=floppy_read_data();
        st1=floppy_read_data();
        st2=floppy_read_data();
        
        rcy=floppy_read_data();
        rhe=floppy_read_data();
        rse=floppy_read_data();
        bps=floppy_read_data();

        int error = 0;
        
        if(st0 & 0xC0) {
            static const char * status[] =
            { 0, "error", "invalid command", "drive not ready" };
            printf("floppy_do_sector: status = %s\n", status[st0 >> 6]);
            error = 1;
        }
        if(st1 & 0x80) {
            printf("floppy_do_sector: end of cylinder\n");
            error = 1;
        }
        if(st0 & 0x08) {
            printf("floppy_do_sector: drive not ready\n");
            error = 1;
        }
        if(st1 & 0x20) {
            printf("floppy_do_sector: CRC error\n");
            error = 1;
        }
        if(st1 & 0x10) {
            printf("floppy_do_sector: controller timeout\n");
            error = 1;
        }
        if(st1 & 0x04) {
            printf("floppy_do_sector: no data found\n");
            error = 1;
        }
        if((st1|st2) & 0x01) {
            printf("floppy_do_sector: no address mark found\n");
            error = 1;
        }
        if(st2 & 0x40) {
            printf("floppy_do_sector: deleted address mark\n");
            error = 1;
        }
        if(st2 & 0x20) {
            printf("floppy_do_sector: CRC error in data\n");
            error = 1;
        }
        if(st2 & 0x10) {
            printf("floppy_do_sector: wrong cylinder\n");
            error = 1;
        }
        if(st2 & 0x04) {
            printf("floppy_do_sector: uPD765 sector not found\n");
            error = 1;
        }
        if(st2 & 0x02) {
            printf("floppy_do_sector: bad cylinder\n");
            error = 1;
        }
        if(bps != 0x2) {
            printf("floppy_do_sector: wanted 512B/sector, got %d", (1<<(bps+7)));
            error = 1;
        }
        if(st1 & 0x02) {
            printf("floppy_do_sector: not writable\n");
            error = 2;
        }
        
        if(!error) {
            unsigned char *data = (unsigned char *)fdc_dma_buffer;
            
            int count=how_many_until*SECTOR_SIZE;
            int iter=0;
            //printf("CHS: %d %d %d ",cyl, head, sector);
            for (int i = SECTOR_SIZE*(sector-1); i < (SECTOR_SIZE*(sector-1)+SECTOR_SIZE); i++) {//LIMIT IS 0x4800, now we are reading first sector of cylinder
                //printf("%c ",data[i]);
                data_out[iter]=data[i];
                iter++;
                
            }
            fdc_control_motor(motor_off);
            return 0;
        }
        if(error > 1) {
            printf("floppy_do_sector: not retrying..\n");
            fdc_control_motor(motor_off);
            return -2;
        }
        
    }
    printf("couldnt read");
    return -1;
    
}



unsigned char floppy_read_data() {
    for(int i = 0; i < 600; i++) {
        for (int i=0; i<10000; i++){};
        if(0x80 & inb(FDC_MSR)) {
            return inb(FDC_FIFO);
        }
    }
    return 0;
}

void fdc_irq_handle(){
   
    pic_send_eoi(6);
}

int floppy_seek(int cyl, int head){
    fdc_control_motor(motor_on);
    int st0, cyl1;
    for (int i=0; i<10; i++){
        fdc_write_command(FDC_SEEK);
        fdc_write_command(head<<2);
        fdc_write_command(cyl);
        fdc_check_interrupt(&st0, &cyl1);
         if(st0 & 0xC0) {
            continue;
        }

        if(cyl == cyl1) {
           
            fdc_control_motor(motor_off);
            return 0;
        }
    }
}



static const char * drive_types[8] = {
    "none",
    "360kB 5.25\"",
    "1.2MB 5.25\"",
    "720kB 3.5\"",

    "1.44MB 3.5\"",
    "2.88MB 3.5\"",
    "unknown type",
    "unknown type"
};
void fdc_detect_drives(){
   outb(0x70, 0x10);
   unsigned drives = inb(0x71);
   printf(" - Floppy drive 0: %s\n", drive_types[drives >> 4]);
}
void fdc_write_command(uint8_t command){
    for (int i=0; i<100000; i++){
        if (inb(FDC_MSR) & 0x80){
            return (void)outb(FDC_FIFO, command);
        }
    }
    printf("timeout in write command");
    
}

