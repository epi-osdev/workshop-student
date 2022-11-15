# Boot sector documentation

## Introduction

This is the document explaining all the boot sector things. It's splitted into differents parts that explain all the differents parts of the booting process. All the files that we will refers to are in the `src/boot_sector` directory (from root).

## Index

this file is split into differents part to better understanding of the booting process

- [Processor modes](#processor-modes)

## All the differents existing modes <a name="processor-modes"></a>

In the Intel architecture, there are many differents modes that the processor can be in. We will see all the differents modes that we will use in the booting process. the processor can boot in differents modes, 16 bits, 32 bits, 64 bits that named real mode, protected mode and long mode. All the modes have differents features and differents limitations. (here it's some of them)
- [Real mode](#real-mode)
- [Protected mode](#protected-mode)
- [Long mode](#long-mode)

### Real mode (16 bits) <a name="real-mode"></a>

the real mode is the firt mode that we have access when booting the processor. It's a 16 bits mode that have a lot of limitations. The main limitation is that the processor can only access 64KB of memory (because the max amount of addresses is 65,536 bytes (2<sup>16</sup>) that represents 64KB. But with some tricky things you can upgrade to less that 1MB of memory

Here is a list of the quality and limitations of the real mode:
- Cons:
    - Less than 1 MB of RAM is available for use.
    - There is no hardware-based memory protection (GDT), nor virtual memory.
    - There is no built in security mechanisms to protect against buggy or malicious applications.
    - The default CPU operand length is only 16 bits.
    - The memory addressing modes provided are more restrictive than other CPU modes.
    - Accessing more than 64k requires the use of segment register that are difficult to work with.
- Pros
    - The BIOS installs device drivers to control devices and handle interrupt.
    - BIOS functions provide operating systems with a advanced collection of low level API functions.
    - Memory access is faster due to the lack of descriptor tables to check and smaller registers.

### Protected mode (32 bits) <a name="protected-mode"></a>

the protected mode is

### Long mode (64 bits) <a name="long-mode"></a>