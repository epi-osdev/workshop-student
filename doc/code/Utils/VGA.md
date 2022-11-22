# INTRODUCTION

Here is the VGA (Video Graphics Array) driver, it's a driver that allows you to display text on the screen. In the project, it's stored it the `src/utils/VGA` folder and can be used with the [`VGA.h`](../../../src/utils/VGA.h) include.

# TABLE OF CONTENTS

- [How it works ?](#how-it-works)
- [How to use it ?](#how-to-use-it)
- [Functions list](#functions-list)
- [Want to contribute ?](#want-to-contribute)

# How it works ? <a name="how-it-works"></a>

The VGA is provided by the BIOS, so we have to follow some rules, the first things to know is that the VGA is a buffer of `80 x 25` characters starting from the adress `0xB8000`, each characters is 2 bytes long, the first one is the character (`ASCII`) and the second one is the `color` of the character. A color is a 4 bits values (0-15) and the first 4 bits are the background color and the last 4 bits are the foreground color. The color is defined in the [`vga.h`](../../../src/utils/VGA/VGA.h) file.

# How to use it ? <a name="how-to-use-it"></a>

The VGA folder provides an API that can be included to use all the functions. Only the `VGA.h` file is needed to use the VGA driver. It's made this way to have a cleaner code


# Functions list <a name="functions-list"></a>

- [vga_clear_screen](#func-vga-clear-screen)

## vga_clear_screen <a name="func-vga-clear-screen"></a>

This function clear fill all the screen with the character ' ' and the color 0x00.
Here is the prototype of the function:

```c
void vga_clear_screen();
```

# Want to contribute ? <a name="want-to-contribute"></a>

If you want to add a function in the VGA driver, you can do it by creating a new file in the `src/utils/VGA` folder and add the function in the [`VGA.h`](../../../src/utils/VGA.h) file. Pay attention to add the documentation of the function in this folder. And please make clean commented code with a proof of working code.

