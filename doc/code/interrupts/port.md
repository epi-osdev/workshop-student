# INTRODUCTION

This file describes the code in the [port.c](../../../src/interrupts/port.c) file.
You can see in this file two functions that communicate with the PIC.
See the `PIC` [doc](pic.md) for more details.
We have two functions, one to write in a port and one to read from a port. Both of them are using the `out` and `in` asm instructions. So we need to call in C the `asm` function to use them (If you want more about the `asm` function, see the this link: [asm](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)).

Here is the reading function:
```c
unsigned char port_byte_in(unsigned short port)
{
    unsigned char result = 0;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return (result);
}
```
It launches the `in` instruction with the `port` as the first argument and the `result` as the second argument. The `result` is the value we want to read from the port.

The writing function is similar:
```c
void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
```
It launches the `out` instruction with the `data` as the first argument and the `port` as the second argument. The `data` is the value we want to write in the port.

If you do not understand all this asm bullshit (it's normal), please see the `asm` keyword documentation: [asm](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)