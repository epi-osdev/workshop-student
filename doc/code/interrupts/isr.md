# INTRODUCTION <a name="introduction"></a>

This document explains the [isr.c](../../../src/interrupts/isr.c) file.
This file is currently initializing and filling the `IDT` with the callbacks defined in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file (doc [here](interrupts_asm.md)). The meaning of this file is to setup all the information and routines to handle interrupts and keyboard inputs.

The code in this file is handling a function `isr_init` that do the following things:
- fill the `IDT` with the callbacks defined in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file
- remap the PIC (Programmable Interrupt Controller), see [pic.md](pic.md) for more information
- enable all the interrupts in the `IDT`
- unset the syscall interrupt

There is also an array of function pointers named `isr_handler` that will contains all the functions that we want to call when an interrupt is triggered. For example, if we want to call a function when the keyboard is pressed, we will set the function pointer at the index `33` (the index of the keyboard interrupt).

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [HEADER FILES](#header-files)
- [CODE EXPLAINATION](#code-explaination)

# HEADER FILES <a name="header-files"></a>

The header file [isr.h](../../../src/interrupts/isr.h) contains interesting datas.
The first thing that you can see is the macro
```c
#define KERNEL_CODE_SEG 0x08
```
This macro defines the code segment offset of the `GDT`. (see [gdt.md](../boot/gdt.md) for more information).

There is many cursed lines in the header file, they are all shaped like this:
```c
extern void isr0();
extern void isr1();
// ...
extern void isr255();
extern void irq0();
extern void irq1();
// ...
extern void irq15();
```
all those lines are basic function prototypes, but they have a special keyword `extern`. This keyword is used to tell the compiler that the function is defined somewhere else. In this case, the functions are defined in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file (doc [here](interrupts_asm.md)).
All the functions will fill the `IDT`

After all the function prototypes, there is a structure named registers_t, they will contains all the registers of the CPU that we pushed in the file [interrupts.asm](../../../src/interrupts/interrupts.asm) with the instruction `lidt` (doc [here](interrupts_asm.md)).
The structure that will handle all the registers is the following:
```c
typedef struct registers_s {
    uint32_t ds;
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax;
    uint32_t interrupt, error;
    uint32_t eip, cs, eflags, esp, ss;
} __attribute__((packed)) registers_t;
```

The last thing that we can see in the header file is the following typedef:
```c
typedef void (*isr_callback)(registers_t *regs);
```
It's creating a type named isr_callback that is a pointer to a function that takes a pointer to a registers_t structure as parameter and returns nothing.

# CODE EXPLAINATION <a name="code-explaination"></a>

We saw the header file with all the declarations, now we will see the code of the file [isr.c](../../../src/interrupts/isr.c).

The first line that we can see is the following:
```c
static isr_callback isr_handlers[MAX_IDT_ENTRIES];
```
This is a global variable that has the keyword `static` (see [here](https://www.geeksforgeeks.org/static-keyword-c/)). 
This variable is handling all the functions that we want to call when an interrupt is triggered. When an interrupts is called, all the functions named `isr` or `irq` (defined in [interrupts.asm](../../../src/interrupts/interrupts.asm) (doc [here](interrupts_asm.md))) will call the function `isr_handler` (or `irq_handler`) with the index of the interrupt as parameter. And the handler will call the function at the index of the interrupt in the `isr_handlers` array.
The `MAX_IDT_ENTRIES` is defined in the header file [isr.h](../../../src/interrupts/isr.h) and is equal to `256` that is the maximum number of interrupts that we can handle.

The first function that we see is:
```c
static void isr_init_idt_gates();
```
This function is filling all the 256 entries of the `IDT` with the callbacks defined in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file that we externalized in the [isr.h](../../../src/interrupts/isr.h) (doc [here](interrupts_asm.md)).
This is typically what we can see in the function:
```c
// ...
set_idt_gate(34, irq2, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
set_idt_gate(35, irq3, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
// ...
set_idt_gate(56, isr56, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
set_idt_gate(57, isr57, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
// ...
```

Next we have the function `isr_init` 
```c
void isr_init()
{
    isr_init_idt_gates();
    pic_remap();
    idt_enable_gates();

    unset_gate_flag(0x80, IDT_FLAG_PRESENT);
}
```
it's doing the following things:
- call the function `isr_init_idt_gates` to fill the `IDT`
- remap the PIC (Programmable Interrupt Controller), see [pic.md](pic.md) for more information
- enable all the interrupts in the `IDT`
- unset the syscall interrupt, We are unsetting the syscall interrupt because we do not have syscall implemented yet, so we don't want to trigger an interrupt when the syscall is called.

The next function is the `isr_handler` it's a function that is called by the `isr_common_stub` function. Here is the implementation:
```c
void isr_handler(registers_t *regs)
{
    uint32_t interrupt = regs->interrupt;

    if (isr_handlers[interrupt] != 0) {
        isr_handlers[interrupt](regs);
    }
}
```
it's just checking is an handler is set in the global `isr_handlers`, and if it is, it's calling it.

The next function is just a setter for the `isr_handlers` array:
```c
void isr_register_handler(int interrupt, isr_callback callback)
{
    isr_handlers[interrupt] = callback;
    set_gate_flag(interrupt, IDT_FLAG_PRESENT);
}
```

And the last one is a bit more complicated, it's the `irq_handler` function:
```c
void irq_handler(registers_t *regs)
{
    if (regs->interrupt >= 40) {
        port_byte_out(0xa0, 0x20);
    }
    port_byte_out(0x20, 0x20);
    if (isr_handlers[regs->interrupt] != 0) {
        isr_handlers[regs->interrupt](regs);
    }
}
```
This function is called by the `irq_common_stub` function. It's doing the following things:
- if the interrupt is greater than 40, it's sending an EOI (End Of Interrupt) to the slave PIC (Programmable Interrupt Controller), see [pic.md](pic.md) for more information
- it's sending an EOI to the master PIC (Programmable Interrupt Controller), see [pic.md](pic.md) for more information
- it's calling the handler if it's set
