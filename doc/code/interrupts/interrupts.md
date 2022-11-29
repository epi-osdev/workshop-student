# INTRODUCTION <a name="introduction"></a>

This document explains you how idt/isr is working and how it's implemented in the project.
The ISR (Interrupt Service Routine) and the IDT (Interrupt Descriptor Table) two importants parts of the PM. The ISR is the function that is called when an interrupt is triggered. The IDT is the table that contains the address of the ISR and the privilege level of the interrupt.
An interrupt is a signal that is sent to the CPU when an event occurs. The CPU can't handle all the interrupts at the same time, so it has to prioritize them. The priority is defined by the privilege level of the interrupt. The higher the privilege level, the higher the priority. The privilege level is defined in the IDT. There is also an other type of interrupts that's called IRQ, the difference between IRQ and ISR is that the IRQ is handled by the PIC (Programmable Interrupt Controller) and the ISR is handled by the CPU. The PIC is a hardware that is used to handle multiple interrupts at the same time. The PIC is connected to the CPU through the IRQ lines. The PIC has 16 IRQ lines, so it can handle 16 interrupts at the same time.
To be simple, the ISR is the function that is called when an interrupt is triggered by the CPU and the IRQ is the function that is called when an interrupt is triggered by the PIC (keyboard, mouse, etc...).
ISR and IRQ have the same data structures

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [How it works](#how-it-works)
- [How to add an ISR](#how-to-add-an-isr)
- [Our implementation](#our-implementation)

# How it works <a name="how-it-works"></a>

Like I explained in the [introduction part](#introduction). An ISR is a callback function that's being called when an interrupt is triggered. All thoses callbacks are stored in the IDT. The IDT is a table likes the GDT. It contains many entries. Each entry contains the address of the ISR and the privilege level of the interrupt.
To be more precise, an entry looks like this:

- 16 bits: `low bytes` of the ISR address
- 16 bits: `segment selector` (the GDT offset of the segment)
- 8 bits: `reserved` (always 0)
- 8 bits: `privilege level` (flags)
- 16 bits: `high bytes` of the ISR address

The IDT has a maximum size of 256 entries. Each entry is 8 bytes long. So the IDT has a maximum size of 2048 bytes.
You have to know that the firsts 32 entries are mandatory. They are used by the CPU to handle the basic exceptions. 
When you had setup all the entries, you have to load the IDT in the CPU. To do that, you have to use the `lidt` instruction. This instruction takes a `IDT descriptor` as parameter. The `IDT descriptor` is a structure that contains the address of the IDT and the size of the IDT -1. The structure looks like this:

- 16 bits: `size` of the IDT -1
- 32 bits: `address` of the IDT

it's a structure of 6 bytes.

# How to add an ISR <a name="how-to-add-an-isr"></a>

To add an ISR in theory you just have to create an entry in the interrupt index that you want and load the idt with `lidt`

# Our implementation <a name="our-implementation"></a>

We implemented the ISR/IDT with three files

- [interrupts.asm](../../../src/interrupts/interrupts.asm): It's the file containing the ISR setup see the [doc](interrupts_asm.md) for more information
- [isr.c](../../../src/interrupts/isr.c): It's the file containing the ISR callback functions and the initialization of all the ISR.
- [idt.c](../../../src/interrupts/idt.c): It's the file containing the IDT setup and the initialization of all the IDT entries.

