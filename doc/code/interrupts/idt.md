# INTRODUCTION <a name="introduction"></a>

This file is explaining how is working the [idt.c](../../../src/interrupts/idt.c). This file is holding our `IDT` and it's providing functions to manipulate it.

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [HEADERS](#headers)
  - [DATA STRUCTURES](#data-structures)
  - [FUNCTIONS](#functions)
- [CODE EXPLANATION](#code-explanation)
  - [idt_init](#idt_init)
  - [idt_set_gate](#idt_set_gate)
  - [idt_load](#idt_load)

# HEADERS <a name="headers"></a>

## DATA STRUCTURES <a name="data-structures"></a>

In the header we are defining some useful datas. The first one that we can see is the:
```c
#define MAX_IDT_ENTRIES 256
```
it defines the max size of our `IDT`. The second interesting thing is the `idt_entry_t` structure. It's defining how an entry of our `IDT` will look like. Here is the structure:
```c
typedef struct idt_entry_s {
    uint16_t base_low;
    uint16_t segment;
    uint8_t always_null;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;
```
It describes an entry of our `IDT`. All the explaination about the fields of this structure can be found in the [interrupts.md](../../../doc/code/interrupts/interrupts.md) file.
The other interesting structure is the `idt_descriptor_t` structure. It's defining how our `IDT` descriptor will look like. Here is the structure:
```c
typedef struct idt_descriptor_s {
    uint16_t size;
    idt_entry_t *idt_start;
} __attribute__((packed)) idt_descriptor_t;
```
It's the `IDT` descriptor, you already know what a descriptor is with the [GDT](../boot/gdt.md) descriptor, it's the same things. The `size` field is the size of our `IDT` and the `idt_start` field is the address of the first entry of our `IDT`.
Those two structures have already been explained in the [interrupts.md](interrupts.md) file. But it's a good thing to have a quick look at them.

There is another data that useful. It's an enum named `IDT_FLAGS`. This enum is containing all the flags that we can use in our `IDT` entries. Here is the enum:
```c
enum IDT_FLAGS {
    // Gate
    IDT_FLAG_GATE_TASK          = 0X5,
    IDT_FLAG_GATE_16BIT_INT     = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP    = 0x7,
    IDT_FLAG_GATE_32BIT_INT     = 0xe,
    IDT_FLAG_GATE_32BIT_TRAP    = 0xf,
    // Ring
    IDT_FLAG_RING0              = (0 << 5),
    IDT_FLAG_RING1              = (1 << 5),
    IDT_FLAG_RING2              = (2 << 5),
    IDT_FLAG_RING3              = (3 << 5),
    IDT_FLAG_PRESENT            = 0x80
};
```

## FUNCTIONS <a name="functions"></a>

The rest of the header is just function prototypes. Here is the prototypes list:

- [idt_init](#func-idt-init)
- [set_idt_gate](#func-set-idt-gate)
- [set_gate_flag](#func-set-gate-flag)
- [idt_enable_gates](#func-idt-enable-gates)
- [unset_gate_flag](#func-unset-gate-flag)
- [lidt](#func-lidt)

### idt_init <a name="func-idt-init"></a>

This function is initialzing our `IDT`, this function is loading all the `IDT` with the `lidt` instruction. Here is the function prototype:
```c
void idt_init();
```

### set_idt_gate <a name="func-set-idt-gate"></a>

This function is setting an entry of our `IDT`. Here is the function prototype:
It takes 4 parameters:
- `uint8_t index`: the index of the entry that we want to set (0 to 255)
- `void *base`: the address of the function that we want to call when the interrupt will be triggered
- `uint16_t segment`: the segment selector of the code segment that we want to call (0x08 for kernel code segment by default)
- `uint8_t flags`: the flags that we want to set for this entry (you can use the `IDT_FLAGS` enum to set the flags)
Here is the function prototype:
```c
void set_idt_gate(uint8_t index, void *base, uint16_t segment, uint8_t flags);
```

### set_gate_flag <a name="func-set-gate-flag"></a>

The set_gate_flag function is setting a flag for an entry of our `IDT`. It's taking 2 parameters:
- `uint8_t index`: the index of the entry that we want to set the flag (0 to 255)
- `uint8_t flag`: the flag that we want to set (you can use the `IDT_FLAGS` enum to set the flags)
Here is the function prototype:
```c
void set_gate_flag(uint8_t index, uint8_t flag);
```

### idt_enable_gates <a name="func-idt-enable-gates"></a>

This function enables all the interrupts of our `IDT`. It's taking no parameters but it changes all the flags of our `IDT` entries. Here is the function prototype:
```c
void idt_enable_gates();
```

### unset_gate_flag <a name="func-unset-gate-flag"></a>

This function unsets a flag for an entry of our `IDT`. It's taking 2 parameters:
- `uint8_t index`: the index of the entry that we want to unset the flag (0 to 255)
- `uint8_t flag`: the flag that we want to unset (you can use the `IDT_FLAGS` enum to set the flags)
Here is the function prototype:
```c
void unset_gate_flag(uint8_t index, uint8_t flag);
```

### lidt <a name="func-lidt"></a>

This function is a wrapper for the `lidt` instruction. The `lidt` instruction doesn't have an equivalent function in C. So we are using this function to load our `IDT` descriptor.
Here is the function prototype:
```c
void lidt();
```