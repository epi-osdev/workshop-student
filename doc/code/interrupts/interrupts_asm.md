# INTRODUCTION <a name="introduction"></a>

This document explains you how we implemented the ISR/IDT PM system in the project. For now we are focussing on the file [interrupts.asm](../../../src/interrupts/interrupts.asm). This file will contains the ISR definitions. It's where the callbacks are called.
The meaning of this file is to defining all the callbacks and to call C functions to be able to use the C language. 

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [How it works](#how-it-works)
- [Code explaination](#code-explaination)

# How it works <a name="how-it-works"></a>

This file is working with PM (`[bits 32]` line). It contains 2 `macro` that will defines all our ISR. They are 2 differents definition of ISR (given by documentation).
the label `isr_common_stub` is here to collect the informations about the interrupts (like the error code, the interrupt number and others...) and to call the C function that will be treated by our code.

So if I'm not wrong, this is the list of the actual path of an interrupts:
- the interrupt `x` is triggered by the CPU.
- the label `isrx` is called.
- the label `isr_common_stub` is called to do the conversion into C.

# Code explaination <a name="code-explaination"></a>

Here is the exlplaination of the code:

```nasm
[bits 32]
[extern isr_handler]
```
You already know the first line. We are working in PM. The second line is to tell the assembler that the function `isr_handler` is defined in another file. Actually it's a C function that we defined in the [isr.c](../../../src/interrupts/isr.c) file.

Next we have our first label called `isr_common_stub`. Here is the beginning
```nasm
isr_common_stub:
    pusha

    xor eax, eax
    mov ax, ds
    push eax                ; save the data segment descriptor

    mov ax, 0x10            ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp ; push esp --> C function parameter
    call isr_handler
    add esp, 4
```
So, let's see the behave of the code line by line:
- `pusha` is used to save all the registers.  We are solving all the register in the stack because we are going to call a C function and in C the arguments are stored in the stack.
- `xor eax, eax` is used to set the value of `eax` to 0 (because xor). It's better to do that because we are cleaning the register before using it.
- `mov ax, ds` is used to move the value of `ds` into `ax`.
- `push eax` is used to push the value of `eax` into the stack. We are saving the value in the stack because we don't know what's happening in the C function, so the value of `eax` can be changed.
- `mov ax, 0x10` is used to move the value `0x10` into `ax`. 0x10 (in decimal: 16) is the index of the kernel data segment descriptor. It's the offset of the `GDT` to access to the kernel data segment descriptor.
- the next 4 lines are used to set the value of `ds`, `es`, `fs` and `gs` to `0x10`. It's the kernel data segment descriptor. It's a security to be sure that the C function will not access to the user memory.
- `push esp` is used to push the value of `esp` into the stack. It will be the first argument of our function, it's the address of the stack where the registers are saved.
- `call isr_handler` is used to call the C function `isr_handler`. It's a function defined in the [isr.c](../../../src/interrupts/isr.c) file that will handle our interrupts.

Above you have the explaination of all the processus of the before calling the C, there is many security code to be sure that the C function will not access forbidden memory or register. Also we are pushing all our datas in the stack to be able to use the C language. The next lines are used to restore all the registers.

```nasm
    add esp, 4

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    sti
    iret
```
- `add esp, 4` is used to add 4 to the value of `esp`. It's used to remove the first argument of the C function (the address of the stack where the registers are saved).
- `pop eax` is used to pop the value of `eax` from the stack. It's the value of the data segment descriptor that we saved before calling the C function.
- The next 4 lines are used to restore the value of `ds`, `es`, `fs` and `gs` to the value of `eax`. It's the value of the data segment descriptor that we saved before calling the C function.
- `popa` is used to restore all the registers.
- `add esp, 8` is used to add 8 to the value of `esp`. It's used to remove the error code and the interrupt number that we saved before calling the C function.
- `sti` is used to enable the interrupts. It's used to be sure that the interrupts are enabled after the C function.
- `iret` is used to return from the interrupt. It's used to be sure that the CPU will continue to execute the next instruction after the interrupt.

Now we have the explaination of the `isr_common_stub` label. Now we are going to see the explaination of the `macro` that we defined. The first one is `ISR`:
```nasm
%macro ISR 1
  global isr%1

  isr%1:
    cli
    push 0 ; dummy error
    push %1 ; interrupt number
    jmp isr_common_stub
%endmacro
```
The syntax to create a macro in asm is the first `%macro` keyword, it's handling the name of the macro and the number of arguments. The macro is ending by the keyword `%endmacro`.
Here is the content of the macro:
- `global isr%1` is used to define the label `isr%1` as global. It's used to be able to call the label from another file. The `%1` is the first argument of the macro. It's the number of the interrupt.
- `isr%1:` is the label of the interrupt.
- `cli` is used to disable the interrupts. It's used to be sure that the interrupts are disabled before calling the C function (and to be sure that the C function will not be interrupted).
- `push 0` is used to push the value `0` into the stack. It's the error code. It's a dummy value because we don't have any error code for the interrupts.
- `push %1` is used to push the value of the first argument of the macro into the stack. It's the interrupt number.
- `jmp isr_common_stub` is used to jump to the label `isr_common_stub`. It's the label that we defined before.

The second macro is `ISR_ERROR`, it's globaaly the same as the `ISR`, the main difference is that we are only pushing the interrupt number and not the error code because it's already pushed by the CPU.
```nasm
%macro ISR_ERROR 1
  global isr%1

  isr%1:
    cli
    push %1 ; interrupt number
    jmp isr_common_stub
%endmacro
```

All the other lines is for setupping the 256 existing interrupts.
```nasm
ISR 0
ISR 1
# ...
ISR_ERROR 10
ISR_ERROR 11
ISR_ERROR 12
# ...
ISR 255
```

If you understand the ISR implementation, you can easily understand the IRQ implementation. The main difference is that we are using a macro with 2 arguments.
