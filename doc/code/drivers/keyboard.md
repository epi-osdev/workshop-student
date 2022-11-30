# INTRODUCTION <a name="introduction"></a>

This file explains you the code in the [keyboard.c](../../../src/drivers/keyboard.c) file.
All this code takes the data from the `PIC` and manages it to be able to use it later in the kernel.
After initializing all the routines, `IDT` and `PIC`, we can now use the keyboard. We can get the key pressed with the `port_byte_in` function. This function is in the [port.c](../../../src/interrupts/port.c) file. See the [doc](port.md) for more details. When a key is pressed it will check the port `0x60` and return the value of the key pressed. We can then use this value to do what we want. If no key is pressed, it will return `0`.
It will not return the ASCII value but it will return the `scancode` of the key pressed. The `scancode` is the value of the key pressed (similar to an index). To transform this code into an ASCII value we are using a layout it will make the transfert between the `scancode` and the ASCII value. You already hear about layouts: `AZERTY`, `QWERTY` is existing layouts.

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [CODE EXPLANATION](#code-explanation)

# CODE EXPLANATION <a name="code-explanation"></a>

We can see in this file the global that represents the current layout:
```c
static KEYBOARD_LAYOUT current_layout = FR;
```
It's a static variable to makes it private to this file. It's the current layout used by the keyboard. It's initialized to `FR` (French layout). the type `KEYBOARD_LAYOUT` is an enum that contains all the layouts available. It's defined in the [keyboard.h](../../../src/drivers/keyboard/keyboard.h) file. For now it only contains this:
```c
typedef enum {
    FR,
    US
} KEYBOARD_LAYOUT;
```
This enum is showing us all the existing layouts. But the definition of the layouts are in another global that looks like this:
```c
static char layouts[LAYOUTS_NUM][KEYS_NUM] = {
    #include "layouts/fr.txt"
    #include "layouts/us.txt"
};
```
It's another static that contains an array of string (string is an array of char). To be more explicit, We are creating an array of `LAYOUTS_NUM` lines and each line contains `KEYS_NUM` char. `LAYOUTS_NUM` and `KEYS_NUM` are both macro defined in the [keyboard.h](../../../src/drivers/keyboard/keyboard.h). `LAYOUTS_NUM` is equal to `2` and `KEYS_NUM` is equal to `58`. They are arbitrary values. 

In this file we also have getter and setter to get and set the current layout
```c
void set_layout(KEYBOARD_LAYOUT layout)
{
    current_layout = layout;
}

KEYBOARD_LAYOUT get_layout()
{
    return current_layout;
}
```
Nothing special to say here.
The interesting things are here:
```c
static uint8_t resolve_scancode(uint8_t scancode)
{
    if (scancode > KEYS_NUM) {
        return 0;
    }

    return layouts[current_layout][scancode];
}

static void callback(registers_t *regs)
{
    uint8_t scancode = port_byte_in(0x60);
    uint8_t pressed_char = resolve_scancode(scancode);

    // TODO
    vga_putchar_at(pressed_char, 0x0f, 20, 20);
}
void init_keyboard()
{
    isr_register_handler(33, callback);
}
```
The `init_keyboard` function is called once in our program, in the [kernel.c](../../../src/kernel.c) file. It registers the `callback` function to the `33` interrupt. The `33` interrupt is the interrupt that is called when a key is pressed. The `callback` function is called when a key is pressed. It will get the `scancode` of the key pressed (The scancode is provided in the `PIC` port `0x60`) and then it will resolve it to get the ASCII value of the key pressed. It will then print the ASCII value on the screen. It's a very simple example of what we can do with the keyboard. We can do a lot of things with it. We can create a shell, a text editor, a game, etc. It's up to you to do what you want with it. The function `resolve_scancode` is here to check if the scancode exists in our arrays
