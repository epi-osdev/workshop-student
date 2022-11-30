# INTRODUCTION <a name="introduction"></a>

The interrupts are the main parts of the communication between the CPU and the kernel. But what are they? How do they work? How can we use them? This document will try to answer these questions.

What are interrupts ?
When you are doing something wrong like dividing by zero, the CPU will send an interrupt to the kernel. The kernel will be processed in priority order. This is the main usage of the interrupts, but you can also program your own interrupts. `ISR` (Interrupts Sub Routine) is the interrupts used by the CPU. `IRQ` (Interrupt Request) is the interrupts used by the hardware (programmable interrupts). `ISR` and `IRQ` are managed by the `IDT` (Interrupt Descriptor Table). It's two names for the same thing.

How do they work ?
In PM there are `256` possibles interrupts. The first `32` are reserved for the CPU (to handle basic things as the division by zero). The other `224` are free to use. We cannot program the behaving of the interrupts but we can define `callbacks`, a callback is a function that will be called when the interrupt is triggered. With this, when the interrupt x is triggered, the callback x will be called.

How can we use them ?
To use the interrupts, you need to define a datastructure named `IDT`, it's an array of entries describing each 256 possible entries. Each entry is composed of a pointer to the callback and some flags. When your `IDT` is ready, you need to load it with the `lidt` instruction. After that, you can enable the interrupts with the `sti` instruction. Now, when an interrupt is triggered, the callback will be called.

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [IMPLEMENTATION](#implementation)
- [FILES](#files)

# IMPLEMENTATION <a name="implementation"></a>

To be easier to understand we are implementing the `IDT` in C. Like I said previously, the IDT is just a table of entries. An entry in the `IDT` is shaped like this:
- `base_low`: The lower 16 bits of the address to jump to when this interrupt fires. (2 bytes)
- `selector`: Kernel segment selector. (2 bytes)
- `zero`: This must always be zero. (1 byte)
- `flags`: More information about the entry. (1 byte)
- `base_high`: The upper 16 bits of the address to jump to. (2 bytes)

In total an entry is 8 bytes. The C implementation can be this (Actually defined [here](../../../src/interrupts/idt.h)):
```c
typedef struct idt_entry_s {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;
```
If you don't know what `__attribute__((packed))` does, it's just to tell the compiler to not add padding between the fields of the structure. It's really useful here because we want to have a structure of 8 bytes and not more. More information about this can be found [here](https://gcc.gnu.org/onlinedocs/gcc-4.8.1/gcc/Type-Attributes.html).

the `idt_entry_t` is just a structure of one entry, but we need a table of entries. It also must be used by many functions (for example to set an entry, a flag, remove an entry, etc...). So we need to make it global. In the file [idt.c](../../../src/interrupts/idt.c) we can see this:
```c
static idt_entry_t idt_entries[MAX_IDT_ENTRIES];
```
We are creating a global named `idt_entries` that is an array of `idt_entry_t` of size `MAX_IDT_ENTRIES`. `MAX_IDT_ENTRIES` is defined in the file [idt.h](../../../src/interrupts/idt.h) and is equal to `256`.
```c
#define MAX_IDT_ENTRIES 256
```
We are using the `static` keyword to make it private to the file. It's not really necessary but it's a good practice to do it.

Also like the `GDT`, we need a structure to describe the `IDT`, it must contains:
- `limit`: The size of the `IDT` minus one. (2 bytes)
- `base`: The address of the first element of the `IDT`. (4 bytes)

This descriptor must be passed to the `lidt` instruction. In C, the implementation can be this:
```c
typedef struct idt_descriptor_s {
    uint16_t limit;
    idt_entry_t *idt_start;
} __attribute__((packed)) idt_descriptor_t;
```
The `idt_entry_t *idt_start` is a pointer to the `IDT`, a pointer is an address of 4 bytes. We can change the type of the pointer to `uint32_t` if we want to be more precise, but it's not really necessary and it's more understandable like this.

Once we have our `IDT` we can create functions to set an entry, remove an entry, set a flag, etc... In the file [idt.c](../../../src/interrupts/idt.c). All the documentation can be found in the file [idt.md](idt.md)

Once we had set the shape of our `IDT`, we can fill it with our callbacks, the definitions of the callbacks can be found in the file [interrupts.asm](../../../src/interrupts/interrupts.asm). The documentation can be found in the file [interrupts.md](interrupts.md). Those callbacks are given to the `IDT` in the file [isr.c](../../../src/interrupts/isr.c). The documentation can be found in the file [isr.md](isr.md).

# FILES <a name="files"></a>

- [idt.c](../../../src/interrupts/idt.c): It's describing the `IDT` and it's providing functions to manipulate it.
- [isr.c](../../../src/interrupts/isr.c): This is the file that contains all the initialization of our `IDT`. So it will call the functions to set the entries, set the flags, etc... (It's called `ISR` but in fact it contains all the interrupts, not only the `ISR`).
- [interrupts.asm](../../../src/interrupts/interrupts.asm): This file contains all the callbacks that will be called when an interrupt is triggered. It must be in assembly because we have data that are not accessible directly in C, like the code errors or the interrupt number.
