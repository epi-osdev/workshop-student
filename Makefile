NASM			= nasm
CC				= i686-elf-gcc
LD				= i686-elf-ld
LD_FLAGS		= -Ttext 0x1000 --oformat binary
QEMU			= qemu-system-x86_64

SRC				= ./src
BIN				= ./bin
ISO				= ./iso
BOOT_SECTOR		= $(SRC)/boot_sector
IMAGE_NAME		= $(ISO)/epi-os.iso

BOOT_SRC		= $(BOOT_SECTOR)/boot_sector.asm
BOOT_BIN		= $(BIN)/boot_sector.bin
BOOT_FLAGS		= -f bin -o $(BOOT_BIN)

C_SRC			= $(SRC)/entry/kernel_entry.c
C_OBJ			= $(C_SRC:.c=.o)

ASM_SRC			= $(SRC)/entry/entry_point.asm
ASM_OBJ			= $(ASM_SRC:.asm=.o)
ASM_FLAGS		= -f elf -g
KERNEL_FILES	= $(ASM_OBJ) \
				$(C_OBJ)
KERNEL_BIN		= $(BIN)/kernel.bin

all: boot_bin kernel_bin
	dd if=$(BOOT_BIN) 					>> $(IMAGE_NAME)
	dd if=$(KERNEL_BIN) 				>> $(IMAGE_NAME)
	dd if=/dev/zero bs=1048576 count=16 >> $(IMAGE_NAME)

run:
	$(QEMU) -fda $(IMAGE_NAME)

kernel_bin: $(KERNEL_FILES)
	$(LD) $(LD_FLAGS) $(KERNEL_FILES) -o $(KERNEL_BIN)

boot_bin:
	$(NASM) $(BOOT_FLAGS) $(BOOT_SRC)

clean:
	$(RM) $(BIN)/*
	$(RM) $(KERNEL_FILES)

re: clean all

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(NASM) $(ASM_FLAGS) $< -o $@
	