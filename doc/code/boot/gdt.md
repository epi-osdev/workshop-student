# Introduction

This document explains all the code in the file `src/boot_sector/gdt.asm`.

## What does it mean?

GDT stands for `Global Descriptor Table`, basically it is a binary structure. It contains entries telling the CPU about memory segments. A similar Interrupt Descriptor Table exists containing task and interrupt descriptors.


## What does it do ?

As we remember, in real mode (16 bits), the segmentation is managed by the segment registers (`cs`, `ds`, `es`, `fs` and `gs`). Now it is managed by the segment stored in the GDT.

The GDT should be seen as a large table with segment descriptors as entries.

Each entry has a size of 8 bytes, remember that the first one is always filled with zeros, it looks like this

```nasm
first_segment:
    dd 0x0
    dd 0x0
```

Every other entries should look like this:

```nasm
entry_example: 
    dw 0xffff       ; Segment limit first 0-15 bits
    dw 0            ; Base first 0-15 bits
    db 0            ; Base 16-23 bits
    db 0x9a         ; Access byte
    db 11001111b    ; High 4 bit flags and the low 4 bit flags
    db 0            ; Base 24-31 bits
```

## Loading the GDT

To load the GDT, there is an instruction named `lgdt`, it means `Load Global Descriptor Table`. `lgdt` requires a 6 bytes structure, in this code we named it `gdt_descriptor`, as we can see below: 
```nasm
lgdt [gdt_descriptor]
```

## Usage example

As example you can see in `src/boot_sector/switch_pm.asm`:
```nasm
jmp CODE_SEG:init_pm
```

Here, `CODE_SEG` is not a reference to the segment itself. Actually, it refers to the segment descriptor entry in the GDT. Moreover, in this part of code we were not in the segment `CODE`, it will make us using a different segment. It is called a `far jump`.