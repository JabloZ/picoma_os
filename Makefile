
export BUILD_DIR=build
export SRC_DIR=src
CC_DIR = /usr/local/i386elfgcc/bin
export PATH := $(CC_DIR):$(PATH)

.PHONY: all floppy_image glibs kernel bootloader clean always

all: floppy_image
#	mkfs.fat -F 12 -n "PICOOS" $(BUILD_DIR)/main_disk.img
floppy_image: $(BUILD_DIR)/main_disk.img

$(BUILD_DIR)/main_disk.img: glibs bootloader kernel
	dd if=/dev/zero of=$(BUILD_DIR)/main_disk.img bs=512 count=2880
	dd if=$(BUILD_DIR)/stage1.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc count=1 seek=0
	dd if=$(BUILD_DIR)/stage2.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=18
	dd if=$(BUILD_DIR)/kernel.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=40
	dd if=test_bins/file_entry.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=3
#	dd if=test_bins/file_entry5.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=4
#	dd if=test_bins/file_entry2.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=1001
#	dd if=test_bins/file_entry3.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=1003
#	dd if=test_bins/file_entry4.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=1002
#	dd if=test_bins/file_entry3.bin of=$(BUILD_DIR)/main_disk.img conv=notrunc seek=990

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
	chmod 666 $(BUILD_DIR)/main_disk.img 
	qemu-system-i386 -m 128M -fda $(BUILD_DIR)/main_disk.img -no-reboot -no-shutdown -monitor stdio
	