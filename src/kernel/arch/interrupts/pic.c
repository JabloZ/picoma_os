#include "pic.h"
//ICW1
#define ICW4_NEEDED 0x01 //
#define CASCADE 0x02 //
#define ADDRESS_INTERVAL 0x04 
#define MODE 0x08
#define ICW1_INIT 0x10

//ICW4
#define ICW4_MODE_8086 0x1
#define ICW4_AUTO 0x02
#define BUF_SLAVE 0x08
#define BUF_MASTER 0x0C
#define FULLY_NESTED_MODE 0x10

#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b

void pic_send_eoi(uint8_t irq){
    if(irq>=8){ // if slave pic
        outb(PIC2_COMMAND,PIC_EOI);
    }
    outb(PIC1_COMMAND,PIC_EOI);
}
void remap_pic(int offset1, int offset2){
    uint8_t a1, a2;
    a1=inb(PIC1_DATA);
    a2=inb(PIC2_DATA);
    
    outb(PIC1_COMMAND, ICW1_INIT | ICW4_NEEDED);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW4_NEEDED);
    io_wait();
    outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(PIC1_DATA, ICW4_MODE_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, ICW4_MODE_8086);
	io_wait();

    outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
};

void disable_pic(){
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff); //mask all interrupts
}

void set_irq_mask(uint8_t irq_num){
    uint16_t port;
    uint8_t value;
    if (irq_num<8){
        port=PIC1_DATA;
    }
    else{
        irq_num-=8;
        port=PIC2_DATA;
    }
    value=inb(port) | (1<<irq_num);
    outb(port, value);
}
void clear_irq_mask(uint8_t irq_num){
    uint16_t port;
    uint8_t value;
    if (irq_num<8){
        port=PIC1_DATA;
    }
    else{
        irq_num-=8;
        port=PIC2_DATA;
    }
    value=inb(port) & ~(1<<irq_num);
    outb(port, value);
}
pic_get_irq_reg(int ocw3){
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return(inb(PIC2_COMMAND)<<8) | inb(PIC1_COMMAND);
}

pic_get_irr(void){
    return pic_get_irq_reg(PIC_READ_IRR);
}
pic_get_isr(void){
    return pic_get_irq_reg(PIC_READ_ISR);
}