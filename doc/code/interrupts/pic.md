# INTRODUCTION <a name="introduction"></a>

This document describes the code in the [pic.c](../../../src/interrupts/pic.c) file.
In this file you will have the pic remaping. They are existing questions about the remaping of the pic. So I will try to explain it here.

What is the PIC ?
The PIC (or 8259_PIC) is one of the most important chips making up the x86 architecture. It holds communication between hardware and software with a thing that we call bus, a bus is just a collection of memory adresses we can read or write in. Due to growing complexity during the time, we need more than one PIC to hold all our hardware devices. We call the addresses of the PICs "ports"

Why do we need to remap it ?
We need to remap the PIC because in the past, the PIC was designed to be used with a 16-bit architecture. Things evolved and we now have a 32-bit architecture. So we need to remap the PIC to avoid conflicts with the memory adresses.

Why was it not fixed with the time ?
Because of the backward compatibility, it's important to keep the old architecture working to avoid breaking old software. So we need to keep this mistake.

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [CODE EXPLANATION](#code-explanation)

# CODE EXPLANATION <a name="code-explanation"></a>

In this file you will find only one function named `pic_remap`, it will do all the remap routine before loading the `IDT`.
```c
void pic_remap()
{
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);
}
```
You can see that we are using the `port_byte_out` function to write in the ports. The first argument is the port number and the second is the value we want to write in the port. You can see more details in the [port.c](../../../src/interrupts/pic.c) file see the [doc](port.md) for more details.

