# INTRODUCTION

This file describes the [isr.c](../../../src/interrupts/isr.c) file, it's containing the initialization of the ISR, the main callback and all the registered callbacks.

# CODE EXPLANATION

This file is containing multiples functions, the first one to be called is the `isr_init` function which initializing all the interrupts. It's shaped like this:
```c
void isr_init()
{
    isr_init_idt_gates();
    pic_remap();
    idt_enable_gates();
    unset_gate_flag(0x80, IDT_FLAG_PRESENT);
}
```
There is 3 functions called in this one, the first one is the `isr_init_idt_gates` which is initializing all the IDT gates, the second one is `pic_remap` that's remaping all the PIC map, the third one is the `idt_enable_gates` which is enabling the IDT gates and the last one is the `unset_gate_flag` which is unsetting the flag of the syscall gate.

Let's see the `isr_init_idt_gates` function:
```c
static void isr_init_idt_gates()
{
    set_idt_gate(0, isr0, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
    set_idt_gate(1, isr1, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
    // ...
    set_idt_gate(32, irq0, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
    set_idt_gate(33, irq1, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
    // ...
    set_idt_gate(47, irq15, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
    set_idt_gate(255, isr255, KERNEL_CODE_SEG, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);
}
```
This function is static because it's only used in this file, it's initializing all the IDT entries with the functions isr0 to isr255. All thoses functions are defined in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file with the `global` keyword. We can use them in the C because we are using the `extern` keyword in the [isr.h](../../../src/interrupts/isr.h) file. It's the same things for the `irq0` to `irq15` functions.

The next function is the `idt_enable_gates` function, it's in the [idt.c](../../../src/interrupts/idt.c) file, you can see the doc [here](idt.md). This function is enabling all the IDT gates.

The last function that's being called is the `unset_gate_flag` function, it's in the [idt.c](../../../src/interrupts/idt.c) file, you can see the doc [here](idt.md). This function is unsetting the flag of the syscall gate. We are unseting the flag 0x80 because in the IDT, the entry 0x80 correspond to the syscall gate, and we don't want to use it (it's our job to create them)

Also, there is 2 more functions in this file: `isr_handler` and `isr_register_handler` the first one is for running some code when an interrupt is triggered, the second one is for registering a callback to the first function. All thoses functions use a global variable named `isr_handlers` which is an array of function pointers.
```c
isr_callback isr_handlers[ENTRIES_NUM];
```
The `isr_callback` and the `ENTRIES_NUM` types are defined in the [isr.h](../../../src/interrupts/isr.h). The ENTRIES_NUM corresponds to the number of entries in the IDT, it's 256.

The functions are shaped like this:
```c
void isr_handler(registers_t *regs)
{
    uint32_t interrupt = regs->interrupt;

    if (isr_handlers[interrupt] != 0) {
        isr_handlers[interrupt](regs);
    }
}
```
The function is taking a parameter regs which is a pointer to a `registers_t` structure, this structure is defined in the [isr.h](../../../src/interrupts/isr.h) file. This structure is containing the registers of the CPU when an interrupt is triggered. The `interrupt` field of the structure is containing the interrupt number.
The parameters contains many information but the important one is the `interrupt` field. It's holding the interrupt number.
The function is checking if the interrupt have a callback, if it's the case, it's calling the callback with the `regs` parameter.


```c
void isr_register_handler(int interrupt, isr_callback callback)
{
    isr_handlers[interrupt] = callback;
    set_gate_flag(interrupt, IDT_FLAG_PRESENT);
}
```
The function is taking 2 parameters, the first one is the interrupt number and the second one is the callback. The function is setting the callback in the `isr_handlers` array and it's setting the flag of the interrupt in the IDT.

There is also an handler for the IRQs, it's the `irq_handler` function, it's shaped like this:
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
This function just handling the IRQs. It's doing doing some stuff with the PICs with the functions `port_byte_out` and `port_byte_in` which are defined in the [port.c](../../../src/interrupts/port.c) file.
