# INTRODUCTION

This is the introduction of all utils, it's all the useful functions that you can use to access some datas and some drivers.
For now it's only including some folders with functions but in the future it will be more complex and more useful. So it will be a good idea to compile it with other makefiles.
An important things about the utils is the fact that only one file is needed to be included in your other projects to use all the functions. So it's **FORBIDDEN** to include any other files.

# TABLE OF CONTENTS

- [VGA](#vga)
- [string](#string)

# VGA <a name="vga"></a>

The vga is the first displaying mode of the PM, it's a `80` x `25` matrix starting from 0xB80000, each characters is 2 bytes long, the first one is the character (`ASCII`) and the second one is the `color` of the character. The color is a 4 bits value, the first 2 bits are the background color and the last 2 bits are the foreground color. The color is defined in the [`vga.h`](../../../src/utils/VGA/VGA.h) file.
All the functions are described [HERE](VGA.md)

# string <a name="string"></a>

The string utils is a string library, it contains all the functions to manipulate strings. All the functions are described [HERE](string.md)
