ARMGNU ?= aarch64-linux-gnu

board ?= rpi4

ifeq ($(board), rpi3)
COPS += -DCONFIG_BOARD_PI3B
QEMU_FLAGS  += -machine raspi3
else ifeq ($(board), rpi4)
COPS += -DCONFIG_BOARD_PI4B
QEMU_FLAGS  += -machine raspi4
endif

# C++ compiler flags
COPS += -g -Wall -nostdlib -Iinclude -I../../shared/baremetal
COPS += -mgeneral-regs-only -ffreestanding -fno-builtin -std=c++17
COPS += -fno-exceptions -fno-rtti

# Assembly flags
ASMOPS = -g -Iinclude -D__ASSEMBLY__

BUILD_DIR = build
SRC_DIR = src

all: benos.bin

clean:
	rm -rf $(BUILD_DIR) *.bin *.elf *.map

$(BUILD_DIR)/%_cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(ARMGNU)-g++ $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%_cpp.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

benos.bin: linker.ld $(OBJ_FILES)
	$(ARMGNU)-ld -T linker.ld -Map benos.map -o $(BUILD_DIR)/benos.elf $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/benos.elf -O binary benos.bin
	cp $(BUILD_DIR)/benos.elf benos.elf

QEMU_FLAGS += -nographic

run:
	qemu-system-aarch64 $(QEMU_FLAGS) -kernel benos.bin

debug:
	qemu-system-aarch64 $(QEMU_FLAGS) -kernel benos.bin -S -s

.PHONY: all clean run debug