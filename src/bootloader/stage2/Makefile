BUILD_DIR?=build
ASM?=nasm
ASMFLAGS?=-f elf32
CC?=/usr/local/i386elfgcc/bin/i386-elf-gcc
CFLAGS?=-ffreestanding -nostdlib -std=c99 -m32 -fno-pie
LINK?=/usr/local/i386elfgcc/bin/i386-elf-ld
LINKFLAGS?=-T linker.ld -nostdlib -m elf_i386 

SOURCES_C=$(wildcard *.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/stage2/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.o, $(SOURCES_ASM))

.PHONY: all stage2 clean always
all: stage2
stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	$(LINK) $(LINKFLAGS) -Map=$(BUILD_DIR)/stage2.map -o $@ $^

$(BUILD_DIR)/stage2/c/%.o: %.c always
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/stage2/asm/%.o: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	mkdir -p $(BUILD_DIR)/stage2/asm
	mkdir -p $(BUILD_DIR)/stage2/c
	
	

clean:
	rm -f $(BUILD_DIR)/stage2.bin