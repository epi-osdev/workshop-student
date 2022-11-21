# Introduction

This document explains all the code in the file `src/boot_sector/switch_pm.md`. It's containing the code to switch from RM to PM. It's called in the [boot_sector.asm](../../../src/boot_sector/boot_sector.asm) file. It's important to switch to PM because it's allowing us to use all the 32 bits features of the CPU, like more 
memory, more registers, etc.
To do the conversion, you need to do some routines. First you need to disable all the interrupts, then you need to load the GDT, then you need to load the IDT, then you need to load the CR3 register, then you need to enable the paging, then you need to enable the protected mode, and finally you need to enable the interrupts.


```nasm
.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32
```
You have to know that to change from real mode to protected mode, you have to do a precise routine and you need to setup many informations. So the first step is to clear the interrupts (see the [DOC](../interrupts/interrupts.md) for more informations). In assembly, the `cli` instruction is doing this.
Then we are loading the gdt (Global Descriptor Table) with the `lgdt` instruction, gdt is a terrible but mandatory thing to setup so you have the documnentation of these shit [HERE](gdt.md).
Then we are setting the cr0 register (Control Register 0) to 1. This is the register that is telling the processor that we are in protected mode.
Finally we are jumping to the `load32` label in the CODE_SEG segment.
