# Boot documentation

## Introduction

this is the documentation of all the booting things, it's splitted into differents parts that explain all the differents parts of the booting process. You have several parts:

## Table of content

- [Boot sector](boot_sector.md)
- [Kernel entry](kernel_entry.md)

## Boot sector

The MD doc:
- [Boot sector](boot_sector.md)

The boot sector is the first part of the booting process. It's serve to load the kernel and give kernel precious informations about the hardware and how the processor should be configured.

## Kernel entry

The MD doc:
- [Kernel entry](kernel_entry.md)

The kernel entry is the entry point of the kernel. It's the first function that is called when the kernel is loaded. It's serve to initialize the kernel and to call the kernel main function. It's the equivalent of the C main function.
