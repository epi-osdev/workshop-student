# Boot sector documentation

## Introduction

This document explains the boot sector stuff. It is splitted into different parts that explain the booting process. Every file that we will refers to are in `src/boot_sector/`.

## Table of content

-   [Processor modes](#processor-modes)
-   [Bios routines](#bios-routines)

## The existing modes <a name="processor-modes"></a>

This is the theorical documentation for the processor modes. In the Intel architecture, there are many different modes that the CPU can use. The CPU can boot in differents modes, **16 bits**, **32 bits** or **64 bits**, respectively named **Real mode**, **Protected mode** and **Long mode**. Every mode has differents features and limitations. More details below:

-   [Real mode](#real-mode)
-   [Protected mode](#protected-mode)
-   [Long mode](#long-mode)

### Real mode (16 bits) <a name="real-mode"></a>

The real mode is the firt mode that we have access when booting the processor. It's a 16 bits mode that have a lot of limitations. The main limitation is that the processor can only access 64KB of memory (because the max amount of addresses is 65,536 bytes (2<sup>16</sup>) that represents 64KB. But with some tricky things you can upgrade to less that 1MB of memory

Here is a list of the quality and limitations of the real mode:

-   Cons:
    -   Less than 1 MB of RAM is available for use.
    -   There is no hardware-based memory protection (GDT), nor virtual memory.
    -   There is no built in security mechanisms to protect against buggy or malicious applications.
    -   The default CPU operand length is only 16 bits.
    -   The memory addressing modes provided are more restrictive than other CPU modes.
    -   Accessing more than 64k requires the use of segment register that are difficult to work with.
-   Pros
    -   The BIOS installs device drivers to control devices and handle interrupt.
    -   BIOS functions provide operating systems with a advanced collection of low level API functions.
    -   Memory access is faster due to the lack of descriptor tables to check and smaller registers.

### Protected mode (32 bits) <a name="protected-mode"></a>

the protected mode is the second mode that we have access when booting the processor. It's a 32 bits mode that have a lot of limitations. The main limitation is that the processor can only access 4GB of memory (because the max amount of addresses is 4,294,967,296 bytes (2<sup>32</sup>) that represents 4GB. But with some tricky things you can upgrade to less that 4GB of memory. \
It's a bit old now but it was for a long time the main mode of the processor. It's still used in some cases like windows 32 bits. The main advantage of the protected mode is that it's a lot more secure than the real mode. It's also a lot more powerful than the real mode. It's also a lot more easy to use than the real mode. But the main cons of the protected mode is that you cannot access to the BIOS interrupts and it's more complex to use but it's not a big deal.

### Long mode (64 bits) <a name="long-mode"></a>

the long mode is the third mode that we have access when booting the processor. It's a 64 bits mode that have a lot of limitations. The main limitation is that the processor can only access 256TB of memory (because the max amount of addresses is 256, terabytes (2<sup>48</sup>) that represents 256TB. But with some tricky things you can upgrade to less that 256TB of memory. \
Don't be affraid of this because you will not have to use this for now :).

## BIOS routine <a name="bios-routine"></a>

When you are starting your computer, the bios is doing some routine to check if in the current disk there is a bootable partition. To check this, it's checking if the address `0x7C00 + 510` has the two bytes `0x55` and `0xAA`. If it's the case, it's loading the first 512 bytes of the disk in the memory at the address `0x7C00` and running the code.

## [boot_sector.asm](../../../src/boot_sector/boot_sector.asm) explaination

Explaination of all the code in the file `src/boot_sector/boot_sector.asm`

```nasm
[org 0x7C00]
[bits 16]
```

Here you have the two first lines that you must understand. The first line is telling the assembler that the code will be at the address `0x7C00`. The second line is telling the assembler that the code will be in 16 bits mode. If you read the [BIOS Routine](#bios-routine) section, you will understand that the bios is loading the first 512 bytes of the disk in the memory at the address `0x7C00`. So we have to put our code at this address.
<br />
After this you have the first label

```nasm
start:
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
```

This label is initializing every segment register to 0 (if you don't know what a segment register is, please see the [**Doc**](../memory/segmentation.md)) \
The `mov ax, 0x00` is moving the value `0x00` in the register `ax`. It's usefull for initializing the registers ds, es and ss to a default value

sp register is the stack pointer, it's storing the address of the top of the stack. We are setting it to `0x7c00` because by definition the stack is growing downwards. The stack base address has been changed (to 0x7C00) to avoid an overwrite over the memory (segments, etc..).

Actually we are in real mode, we want to switch to protected mode. the .load_protection_mode label is doing this.

```nasm
.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32
```
You have to know that to change from real mode to protected mode, you have to do a precise routine and you need to setup many informations. So the first step is to clear the interrupts (see the [DOC](../interrupts/interrupts.md) for more informations). In assembly, the `cli` instruction is doing this. \
Then we are loading the gdt (Global Descriptor Table) with the `lgdt` instruction, gdt is a terrible but mandatory thing to setup so you have the documnentation of these shit [HERE](gdt.md). \
Then we are setting the cr0 register (Control Register 0) to 1. This is the register that is telling the processor that we are in protected mode. \
Finally we are jumping to the `load32` label in the CODE_SEG segment. \

The end of the file is for loading all the other files and writting the magic bytes at the end of the file.

```nasm
%include "src/boot_sector/disk.asm"
%include "src/boot_sector/gdt.asm"

times 510-($ - $$) db 0
dw 0xAA55
```
%include instruction is obviously for including the other files.
- [gdt.asm](gdt.md)
- [disk.asm](disk.md)

the `times 510-($ - $$) db 0` is writing 510 - (current address - start address) bytes of 0. It's just padding the file with 0 to have a size of 510 bytes. The last two bytes are the magic bytes `0xAA55` that are telling the bios that the file is bootable (necessary for the [BIOS routine](#bios-routine)).