
export BUILD_DIR=build
export SRC_DIR=src
CC_DIR = /usr/local/i386elfgcc/bin
export PATH := $(CC_DIR):$(PATH)
CC=/usr/local/i386elfgcc/bin/i386-elf-gcc

.PHONY: all floppy_image kernel bootloader clean always

all: floppy_image
#	mkfs.fat -F 12 -n "PICOOS" $(BUILD_DIR)/main_disk.img
floppy_image: $(BUILD_DIR)/main_disk.img

$(BUILD_DIR)/main_disk.img: bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/main_disk.img bs=512 count=2880
	dd if=$(BUILD_DIR)/stage1.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc 
	mcopy -i $(BUILD_DIR)/main_disk.img -mv $(BUILD_DIR)/stage2.bin "::stage2.bin"
	mcopy -i $(BUILD_DIR)/main_disk.img -mv $(BUILD_DIR)/kernel.bin "::kernel.bin"
	
	mcopy -i $(BUILD_DIR)/main_disk.img test.txt "::test.txt"
	mmd -i $(BUILD_DIR)/main_disk.img "::testd"
	mmd -i $(BUILD_DIR)/main_disk.img "::testd/testd2"
	mcopy -i $(BUILD_DIR)/main_disk.img test.txt "::testd/testd2/test.txt"

 
bootloader: stage1 stage2

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR))

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR))

#kernel_setup: $(BUILD_DIR)/kernel_setup.bin

#$(BUILD_DIR)/k_setup.bin: always
#	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR))

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR))

always:
	
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*

run:
	
	qemu-system-i386 -m 128M -fda $(BUILD_DIR)/main_disk.img -no-reboot -no-shutdown -monitor stdio
