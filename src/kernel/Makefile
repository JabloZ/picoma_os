BUILD_DIR?=build
ASM?=nasm
ASMFLAGS?=-f elf32
CC?=i386-elf-gcc
CFLAGS?=-ffreestanding -nostdlib -std=c99 -m32 -fno-pie
#LINK?=/usr/local/i386elfgcc/bin/i386-elf-ld
LINK?=i386-elf-ld
LINKFLAGS?=-T linker.ld -nostdlib -m elf_i386
LINKLIBS?=/usr/local/i386elfgcc/lib/gcc/i386-elf/12.2.0/libgcc.a
SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/kernel/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/kernel/asm/%.o, $(SOURCES_ASM))

.PHONY: all kernel clean always
all: kernel
kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LINK) $(LINKFLAGS) -Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(LINKLIBS)
$(BUILD_DIR)/kernel/c/%.o: %.c always
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/kernel/asm/%.o: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	
	mkdir -p $(BUILD_DIR)/kernel/asm
	mkdir -p $(BUILD_DIR)/kernel/c
	
	

clean:
	rm -f $(BUILD_DIR)/kernel.bin

