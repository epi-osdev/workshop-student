# INTRODUCTION

This file is explaining the IDT implementation in the kernel in the [idt.c](../../../src/interrupts/idt.c).
This file is for initializing and loding the IDT and their entries.

# CODE EXPLANATION

The file is splitted into different functions and variables.
The first thing that we can see is the declaration of two global static variables:

```c
static idt_entry_t idt_entries[ENTRIES_NUM];
static idt_descriptor_t idt_descriptor = {
    sizeof(idt_entries) - 1,
    idt_entries
};
```
The first variable is an array of `idt_entry_t` which is a structure that represents an entry.
the idt_entry_t is shaped like this:
```c
typedef struct idt_entry_s {
    uint16_t base_low;
    uint16_t segment;
    uint8_t always_null;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;
```
As you can see the idt_entry_t represents the entry of the IDT. It's described in the [interrupts.md](interrupts.md) file.

The second structure is the descriptor of the IDT. It's shaped like this:
```c
typedef struct idt_descriptor_s {
    uint16_t size;
    idt_entry_t *idt_start;
} __attribute__((packed)) idt_descriptor_t;
```
The idt_descriptor_t is used to load the IDT into the CPU. It's described in the [interrupts.md](interrupts.md) file.

Those variables are used in the files functions.
After you filled your IDT, you must tell the CPU to load it. To do that you have the function `idt_init`:
```c
void idt_init(void)
{
    lidt();
}
```
The lidt functions is just calling the `lidt` assembly function. In C you can't call assembly functions, so you have to create a wrapper function.
```c
void lidt(void)
{
    __asm__("lidt %0" : : "m" (idt_descriptor));
}
```
Do not try to understand how it's working because it's not important. The important thing is that it's loading the IDT into the CPU.

You have other functions that are used in the [isr.c](isr.c) file doc [here](isr.md). 

This function is used to set an entry of the IDT, it takes the index of the entry, the callback address, the segment and the flags.
```c
void set_idt_gate(
    int32_t entry_index,
    void *base, 
    uint32_t segment,
    uint8_t flags
)
{
    if (entry_index > (ENTRIES_NUM - 1)) {
        return;
    }

    idt_entries[entry_index].base_low = (uint32_t) base & 0xffff;
    idt_entries[entry_index].segment = segment;
    idt_entries[entry_index].always_null = 0;
    idt_entries[entry_index].flags = flags;
    idt_entries[entry_index].base_high = (uint32_t) base >> 16;
}
```
This function is used to set a flag of an entry of the IDT. It takes the index of the entry and the flag.
```c
void set_gate_flag(int entry_index, uint8_t flag)
{
    if (entry_index > (ENTRIES_NUM - 1)) {
        return;
    }

    idt_entries[entry_index].flags |= flag;
}
```
This function is used to unset a flag of an entry of the IDT. It takes the index of the entry and the flag.
```c
void unset_gate_flag(int entry_index, uint8_t flag)
{
    if (entry_index > (ENTRIES_NUM - 1)) {
        return;
    }

    idt_entries[entry_index].flags &= ~flag;
}
```
This function is used to set the callback address of an entry of the IDT. It takes the index of the entry and the callback address.
```c
void idt_enable_gates()
{
    for (int i = 0; i < ENTRIES_NUM; i++) {
        set_gate_flag(i, IDT_FLAG_PRESENT);
    }
}
```
