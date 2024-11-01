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
int received_int=0;

uint8_t floppy_wait_irq()
{
  while(received_int == 0){
    
  };
  //reset floppy_recieved_int to default (FALSE)
  received_int = 0;
  return 1;
}

void init_fdc(){
    floppy_motor_state=0;
    fdc_detect_drives();
    g_irq_handler[6]=fdc_irq_handle;
    fdc_reset();
    fdc_calibrate(0);

}
int fdc_control_motor(int state){
    
     //we want to turn off
        if (state == motor_on){
            outb(FDC_DOR, 0x1C);
            floppy_motor_state=motor_on;     
            sleep(2000);
        }
   
        if (state==motor_off){
            outb(FDC_DOR, 0x0C);
            floppy_motor_state=motor_off;
            sleep(2000);
        }
    
    return 0;
}
void fdc_check_interrupt(int* st0, int* cyl){
    fdc_write_command(FDC_SENSE_INTERRUPT);
    *st0=inb(FDC_FIFO);
    *cyl=inb(FDC_FIFO);
}
unsigned long virt_to_phys(void *virt_addr) {
    return ((unsigned long)virt_addr) - 0xC0000000;
}

void init_dma(int cmd){
    //memset((&fdc_dma_buffer),'c',512);
    union{
        unsigned char bytes[4];
        unsigned long l;
    }a, c;
    a.l=virt_to_phys(&fdc_dma_buffer);
    c.l=floppy_dma_len-1;
    //memset(virt_to_phys(&fdc_dma_buffer), 'c', 512);
    if((a.l >> 24) || (c.l >> 16) || (((a.l&0xffff)+c.l)>>16)) {
        printf("a.l: %d\n c.l: %d\n combo: %d\n", a.l >> 24, c.l >>16 ,((a.l&0xffff)+c.l)>>16);
        printf("ERROR");
        return 0;
    }
    unsigned char cmd_floppy;
    if (cmd==sector_read){
        //memset((&fdc_dma_buffer), 0, 512);
        cmd_floppy=0x46;
    }
    if (cmd==sector_write){
        //memset((&fdc_dma_buffer), 'c', 512);
        cmd_floppy=0x4a;
    }
   
     //mask dma 2
   
    outb(0x0A, 0x06);
    outb(0x0d, 0xff); //reset

    outb(0x04, a.bytes[0]);
    outb(0x04, a.bytes[1]);
    outb(0x81, a.bytes[2]);
   
    outb(0x0c, 0xff); //reset
    outb(0x05, c.bytes[0]);
    outb(0x05, c.bytes[1]);
    
     outb(0x0b, cmd_floppy);
    outb(0x0A, 0x02);
  
}

int fdc_reset(){
    
    outb(FDC_DOR, 0x0);
    outb(FDC_DOR, 0x0C);
    //fdc_control_motor(motor_on);
    floppy_wait_irq();
    {
        int st0, cyl;
        fdc_check_interrupt( &st0, &cyl);
    }


    outb(FDC_CCR, 0); //1.44 default
    fdc_write_command(3);
    fdc_write_command(0xdf);
    fdc_write_command(0x02);
   
}

int fdc_calibrate(int drive){
    unsigned int st0, cyl=-1;
    fdc_control_motor(motor_on);
    for (int i=0; i<10; i++){

        
        fdc_write_command(FDC_RECALIBRATE);
        fdc_write_command(0);
        //floppy_wait_irq();
        fdc_check_interrupt(&st0, &cyl);

        if (st0 & 0xC0){
            continue;
        }
        if (!cyl){
            fdc_control_motor(motor_on);
            return 0;
        }
    }
   fdc_control_motor(motor_off);
   printf("exhausted 10 tries");
    return -1;
}
int fdc_write_sector(int drive, int lba, uint8_t* data_out, int how_many_until, unsigned char cmd){
    int heads=2;
    int sectors=18;
    
    // DELETE THIS IF, IF SUPPORTED BIGGER FAT(32, 16)
    if (lba>=2880){
        //printf("LBA OUT OF LIMIT FOR FAT12: MAX IS 2879\n");
        return 1;
    }
    unsigned char cmd_floppy;
    cmd_floppy=0x45;
    int cyl = lba / (heads * sectors);       
    int head = (lba / sectors) % heads;      
    int sector = (lba % sectors) + 1;  
  
    fdc_control_motor(motor_on);  
    int rt=floppy_seek(cyl, head);
    if(rt){
        printf("seek failed");
        return 0;
    }
    //memset((uint8_t*)fdc_dma_buffer, 'c', 512);
    memcpy(&fdc_dma_buffer,data_out, 512);
    init_dma(sector_write);
        for (int tries = 0;tries < 20;tries++) {
            //memcpy(fdc_dma_buffer,'b',512);
            fdc_control_motor(motor_on);
            outb(FDC_CCR, 0);

            
            fdc_write_command(cmd_floppy); 
            fdc_write_command(0);//drive
            //return 0;
            
            fdc_write_command(cyl); 
            fdc_write_command(head);//first head
            fdc_write_command(sector);//first sector
            fdc_write_command(2);//bytes per sector, but counted differently
            fdc_write_command(sector+1);//tracks
            fdc_write_command(0x1b); //gap3
            fdc_write_command(0xff); //data len
            
            //fdc_write_command(FDC_SENSE_INTERRUPT);
            if (!floppy_wait_irq()){
                printf("irq fail");
                break;
            }
            //memset(&fdc_dma_buffer, 'c', 512);
            unsigned char st0, st1, st2, st3, reg1, reg2, reg3;
            st0=floppy_read_data();
            st1=floppy_read_data();
            st2=floppy_read_data();
            st3=floppy_read_data();
            reg1=floppy_read_data();
            reg2=floppy_read_data();
            reg3=floppy_read_data();
            //fdc_check_interrupt(&st0, &cyl);
            if (!(st0 & 0xC0)){
                
                return 1;
            }
            fdc_calibrate(0);
        }
        printf("failed after 20 tries");
        return 0;
}

int fdc_read_sector(int drive, int lba, uint8_t* data_out, int how_many_until, unsigned char cmd){
    int heads=2;
    int sectors=18;
    
    // DELETE THIS IF, IF SUPPORTED BIGGER FAT(32, 16)
    if (lba>=2880){
        //printf("LBA OUT OF LIMIT FOR FAT12: MAX IS 2879\n");
        return 1;
    }
    unsigned char cmd_floppy;
    int cyl = lba / (heads * sectors);       
    int head = (lba / sectors) % heads;      
    int sector = (lba % sectors) + 1;  
    if (cmd==sector_read){
        cmd_floppy=0x46;
    }
    else if (cmd==sector_write){
        
        cmd_floppy=0x4a; //0x4a
    }
   
    else{
        printf("niby tu");
        return -1;
    }
   
    if (floppy_seek(cyl, 0)){ return -1;};
    if (floppy_seek(cyl, 1)){ return -1;};
    //printf("%d %d %d", cyl,head,sector);
    //printf("%p",&fdc_dma_buffer);
    fdc_control_motor(motor_on);
    //sleep(1000);
    memset(fdc_dma_buffer, 0, 512);
    
    for (int i=0; i<20; i++){
      
       
        //init_dma(cmd_floppy);
        unsigned char *data = virt_to_phys(fdc_dma_buffer);
        fdc_control_motor(motor_on);
        for (int tries = 0;tries < 3;tries++) {
           
            fdc_control_motor(motor_on);
            init_dma(sector_read);

            outb(FDC_CCR, 0);
            fdc_write_command(cmd_floppy); 
           
            fdc_write_command(0);//drive
            fdc_write_command(cyl); 
            fdc_write_command(head);//first head
            fdc_write_command(sector);//first sector
            fdc_write_command(2);//bytes per sector, but counted differently
            fdc_write_command(sector);//tracks
            fdc_write_command(0x1b); //gap3
            fdc_write_command(0xff); //data len
            //fdc_write_command(FDC_SENSE_INTERRUPT);
            floppy_wait_irq();

            unsigned char st0, st1, st2, st3;
            st0=floppy_read_data();
            st1=floppy_read_data();
            st2=floppy_read_data();
            st3=floppy_read_data();
            printf(" %d %d %d %d\n",st0,st1,st2,st3);
            if (cmd==sector_read){
                int iter=0;
                for (int j=0; j < 512; j++) {
                    data_out[iter]=data[j];
                    iter++; 
                }
                return 0;
            }

        }
    printf("couldnt read");
    fdc_control_motor(motor_off);
    return -1;
    
}}



unsigned char floppy_read_data() {
    for(int i = 0; i < 600; i++) {
        sleep(100);
        if(0x80 & inb(FDC_MSR)) {
            return inb(FDC_FIFO);
        }
    }
    return 0;
}

void fdc_irq_handle(){
   
   received_int=1;
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
void fdc_write_command(unsigned char command){
    for (int i=0; i<600; i++){
        sleep(10);
        if (inb(FDC_MSR) & 0x80){
            outb(FDC_FIFO, command);
            return;
        }
    }
    printf("timeout in write command");
    
}

/*
  
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
*/