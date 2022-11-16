# Introduction

This document explains all the code in the file `src/boot_sector/disk.asm`.
This file manage all the hard disk related stuff.

## Table of content

- [Disk structure](#disk-structure)
- [Disk routines](#disk-routines)
- [Code explaination](#code-explaination)

## Disk structure <a name="disk-structure"></a>

The structure of a hard disk is quite simple, it inherits the real structure of hard disks of that time.
Here is a simple schema of the structure of a hard disk:
<img src="../../img/hard_disk_structure.png" alt="Hard disk structure"/>
You can see that the hard disk is divided in 4 parts, the biggest part is the head, an head contains many cylinders and a cylinder contains many sectors.
Here is a description of what is a head, a cylinder and a sector:

#### Head

The head is the needle that is used to read and write on a disk, so a head represents on which disk you want to write. In asm, the head is represented by the `DH` register.

#### Cylinder

The cylinder is a division of the disk in a vertical way. It's used to represent the position of the head on the disk. In asm, the cylinder is represented by the `CH` register.

#### Sector

The sector is a division of the disk in a horizontal way. It's used to represent the position of the head on the disk. In asm, the sector is represented by the `CL` register.

## Disk routines <a name="disk-routines"></a>

## Code explaination <a name="code-explaination"></a>
