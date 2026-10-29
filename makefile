CC 		= arm-none-eabi-gcc
LD 		= arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
AS 		= arm-none-eabi-as
QEMU	= qemu-system-arm
GHEX 	= ghex
GDB		= gdb-multiarch

C_FLAGS    		 = -mcpu=cortex-m3 -mthumb -Wall -O0 -ffreestanding
DUMP_FLAGS 		 = -D -r
LINK_FLAGS 		 = -T linker.ld -Map=$(KERNEL_MAP)
QEMU_FLAGS 		 = -machine lm3s6965evb -cpu cortex-m3
QEMU_DEBUG_FLAGS = -s -S
GDB_FLAGS		 = -ex "target remote localhost:1234"

SRC_DIR    = ./src
BUILD_DIR  = ./build
OBJECT_DIR = $(BUILD_DIR)/obj
BIN_DIR    = $(BUILD_DIR)/bin
MAP_DIR    = $(BUILD_DIR)/map
DUMP_DIR   = $(BUILD_DIR)/dump

C_SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
C_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJECT_DIR)/%.c.o,$(C_SRC_FILES))

KERNEL_ELF_NAME = kernel.elf
KERNEL_ELF  	= $(BIN_DIR)/$(KERNEL_ELF_NAME)
KERNEL_MAP		= $(MAP_DIR)/$(KERNEL_ELF_NAME).map

KERNEL_BIN		= kernel.bin

.PHONY: all
all: $(BUILD_DIR) $(KERNEL_ELF)

$(KERNEL_ELF): $(C_OBJ_FILES)
	$(LD) $(LINK_FLAGS) $^ -o $@ 

$(OBJECT_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(CC) $(C_FLAGS) -c $< -o $@
	$(OBJDUMP) $(DUMP_FLAGS) $@ > $(DUMP_DIR)/$(notdir $@).dump

$(BUILD_DIR):
	mkdir -p $(OBJECT_DIR) $(BIN_DIR) $(MAP_DIR) $(DUMP_DIR)

show: $(KERNEL_ELF)
	$(GHEX) $<

qemu: $(KERNEL_ELF)
	$(QEMU) $(QEMU_FLAGS) -kernel $(KERNEL_ELF)

qemu-debug: $(KERNEL_ELF)
	$(QEMU) $(QEMU_FLAGS) $(QEMU_DEBUG_FLAGS) -kernel $(KERNEL_ELF)

.PHONY: gdb
gdb:
	$(GDB) $(GDB_FLAGS)

clean:
	rm -rf $(BUILD_DIR)