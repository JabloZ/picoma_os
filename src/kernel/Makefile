BUILD_DIR?=build
ASM?=nasm -g
ASMFLAGS?=-f elf32
CC?=i386-elf-gcc
CFLAGS?=-ffreestanding -nostdlib -std=c99 -m32 -fno-pie -Wall
CFLAGS += -I$(SRCDIR)/kernel
#LINK?=/usr/local/i386elfgcc/bin/i386-elf-ld
LINK?=ld
LINKFLAGS?=-T linker.ld -nostdlib -m elf_i386 
 
LINKLIBS?=/usr/local/i386elfgcc/lib/gcc/i386-elf/12.2.0/libgcc.a
SOURCES_C=$(shell find . -type f -name  "*.c")
SOURCES_ASM=$(shell find . -type f -name  "*.asm")
HEADERS_C=$(shell find . -type f -name  "*.h")
HEADERS_ASM=$(shell find . -type f -name  "*.inc")

OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.o, $(SOURCES_ASM))


.PHONY: all kernel clean
all: kernel

kernel: $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.elf: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LINK) $(LINKFLAGS) -Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(LINKLIBS)

$(BUILD_DIR)/kernel/c/%.o: %.c $(HEADERS_C)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kernel/asm/%.o: %.asm ${HEADERS_ASM}
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) -o $@ $<


clean:
	rm -f $(BUILD_DIR)/kernel

