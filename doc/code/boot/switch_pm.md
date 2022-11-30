# Introduction

This document explains all the code in the file `src/boot_sector/switch_pm.md`. It's containing the code to switch from RM to PM. It's called in the [boot_sector.asm](../../../src/boot_sector/boot_sector.asm) file. It's important to switch to PM because it's allowing us to use all the 32 bits features of the CPU, like more 
memory, more registers, etc. To do the switch you must make a routine that will modify the behavior and switch the CPU into 32 bits mode.

## Table of content

- [BIOS Routine](#bios-routine)
- [Routine explaination](#routine-explaination)
- [Code explaination](#code-explaination)

## BIOS Routine <a name="bios-routine"></a>

We need to do a routine to do the switch, this routine is provided by the Intel doc
Here is the routine to switch from RM to PM:

- Clear the interrupts
- Load the GDT
- Set the first bit of the CR0 register to 1
- Make a far jump to the first 32 bits label in the code segment
- Clear all the old segment registers
- Update the stack pointer

All theses instructions must be done to switch from RM to PM. If one is missing, the switch can fail.

## Routine explaination <a name="routine-explaination"></a>

The first task of the routine is to clear the interrupts, you have to know that in PM it's not possible to use the interrupts because it's made radically different. So we need to disable all the interrupts to avoid conflicts with the new mode and the switching routine.

Next, we need to load the GDT, the memory management is different between RM (basic flat memory) and PM (memory segmentation). The memory segmentation ask first to create a complex data structure that named [GDT](gdt.md)

Next, we are setting the first bit of the CR0 register to 1, it's done by this code:
```nasm
mov eax, cr0
or eax, 1
mov cr0, eax
```
No explication for this (for now), it's juste asked by the Intel doc.

Next, we are making a far jump to the first 32 bits label in the code segment, it's done by this code:
```nasm
jmp CODE_SEG:init_pm
```
You probably now how this type of jump works but there is an important feature to know. The far jump clear the CPU pipeline, the CPU pipeline in modern CPU is used to run tasks in a parallel way. It's very useful but for us when we are changing the mode some tasks can crash because of the differents mode (like 16 bits segmentation, interrupts, etc..). To avoid theses crashes we need to clear the CPU pipeline, it's done by the far jump.

Now we are entering the first PM label. like said previously (#routine-explaination) we need to clear all the old segment because it's useless so we are setting all the segment registers (ds, ss, es, fs, gs) to the data segment. So when using theses, it will refer to the data segment.

Finally, we are updating the stack pointer, before, the stack pointer was in 16 bits mode, now we are in 32 bits mode so we have more space, more memory adress, and new registers to the stack pointer. So we need to update the stack pointers to the new 32 bits mode.

## Code explaination <a name="code-explaination"></a>

we have two different labels in this file, one for RM and the other one for PM. Here is the first one

```nasm
[bits 16]
switch_to_pm:
    cli                     ; 1. disable interrupts
    lgdt [gdt_descriptor]   ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1             ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; 4. far jump by using a different segment
```
this is our 16 bits mode, we are still in RM and as you can see, it's the first steps of the [bios routine](#bios-routine). Once we have done this, we are jumping to the PM label.

```nasm
[bits 32]                   ; protected mode
init_pm:                    ; we are now using 32-bit instructions
    mov ax, DATA_SEG        ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call entry_point        ; 7. Call a well-known label with useful code

```
No more to say here, it's just the second part of the [bios routine](#bios-routine). Once we have done this, we are jumping to the entry_point label. It's were we can finally start to use the 32 bits features of the CPU.
