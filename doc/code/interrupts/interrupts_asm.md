# INTRODUCTION <a name="introduction"></a>

This document describes all the theory and code in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file.

This file is handling all `ISR` callback declarations. Like explained [here](interrupts.md) each `ISR` needs a callback. When the CPU is triggering an interrupt, it will call push information in some asm register and then call a callback. To make things easier, we are first declaring the callback in assembler to make sure we have all the data provided by the CPU. Then we are pushing the data in the stack before calling C functions.

To declare all the callbacks we are using a label and we are using the keyword `global` to make sure the C code can access it. (In the C code, it can be accessed with the `extern` keyword).
We must define 256 callbacks, and many of them are identical. So to make things easier, we are using a macro to define all the callbacks.

Each callback is basically doing the same things:
- clearing the interrupts (with the `cli` instruction)
- pushing the data in the stack
- calling the label `isr_common_stub` (for `ISR`) or `irq_common_stub` (for `IRQ`)

The `isr_common_stub` and `irq_common_stub` are defined in the [interrupts.asm](../../../src/interrupts/interrupts.asm) file. These functions are called by all the callbacks that we have defined. They are doing they are doing the transition between the assembler code and the C code. It's pushing the datas in the stack and then calling the C function.

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [CODE EXPLAINATION](#code-explaination)
  - [ISR CALLBACKS](#isr-callbacks)
  - [IRQ CALLBACKS](#irq-callbacks)
  - [COMMON STUBS](#common-stubs)


# CODE EXPLAINATION <a name="code-explaination"></a>

We need to set 256 `ISR` and 16 `IRQ` callbacks. Barely all of them are identical, the only difference is the number of the interrupt. So we are using a macro to define all the callbacks.

## ISR CALLBACKS <a name="isr-callbacks"></a>

Here is the macro that we are using to define all the `ISR` callbacks:
```nasm
%macro ISR 1
    global isr%1

    isr%1:
      cli
      push 0
      push %1
      jmp isr_common_stub
%endmacro
```
with the keyword `%macro` we are defining a macro, and with the keyword `%endmacro` we are closing the macro. This macro is named `ISR` and takes `1` parameter. This parameter is the number of the interrupt. We can use this parameter with the keyword `%1`.
So, we are declaring a global label named `isr%1` (with the `%1` we are using the parameter of the macro). Then we are clearing the interrupts, pushing `0` and the number of the interrupt in the stack. Finally we are calling the `isr_common_stub` function. It's pushing 0 because the CPU push the error code in the stack only for some interrupts.

Also, specific `ISR` push some data in the stack (error code). So we are using a specific macro to define these callbacks:
```nasm
%macro ISR_ERROR 1
    global isr%1

    isr%1:
      cli
      push %1
      jmp isr_common_stub
%endmacro
```
Globally, it's the same as the previous macro, but we are not pushing `0` in the stack. Because the CPU is pushing the error code in the stack.

## IRQ CALLBACKS <a name="irq-callbacks"></a>

The difference between the `ISR` and the `IRQ` is that the `IRQ` are not triggered by the CPU. They are triggered by the PIC. So we are using a macro to define all the `IRQ` callbacks:
```nasm
%macro IRQ 2
    global irq%1

    irq%1:
      cli
      push %1; dummy error
      push %2 ; interrupt number
      jmp irq_common_stub
%endmacro
```
It's a macro taking 2 parameters. The first one is the number of the interrupt, and the second one is the number of the IRQ. We are using the second one to know which PIC is triggering the interrupt. We are using this information to send an `EOI` to the PIC (End Of Interrupt).

## COMMON STUBS <a name="common-stubs"></a>

A common stub is a function that is called by all the callbacks. It's doing the transition between the assembler code and the C code. It's pushing the data in the stack and then calling the C function.

The `isr_common_stub` and the `irq_common_stub` are the same function. The only difference is the name of the function that is called. We are not using macro for these functions because it's easier when you want to add some code in the function.

```nasm
isr_common_stub: ; or irq_common_stub
    pusha

    xor eax, eax
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call isr_handler ; or irq_handler
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
Here is all the things done by the common stub:
- push all the registers in the stack (with the `pusha` instruction). Pushing into the stack permits to save the registers values and to use them in the C code.
- saving the data segment register (`ds`) in the stack. We will pop it later, it's avoiding C code to modify our data segment register.
- Initializing all the data segment registers (`ds`, `es`, `fs`, `gs`) with the data segment register in our `GDT` (0x10). This is to make sure that the C code can access the data segment register.
- push the stack pointer in the stack, it's very useful because it's a pointer to all the registers that we pushed before.
- call the C function that will manage all the interrupts (`ISR` or `IRQ` are handle in different functions).
- add 4 to the stack pointer, it's because we pushed the stack pointer in the stack before and we want to remove it.
- pop the data segment register that we saved before.
- reset all the data segment registers (`ds`, `es`, `fs`, `gs`) with the data segment register that we saved before.
- pop all the registers in the stack (with the `popa` instruction). Popping from the stack permits to restore the registers values.
- add 8 to the stack pointer, it's because we pushed 2 values in the stack before (the dummy error and the interrupt number).
- enable the interrupts (with the `sti` instruction).
- return from the interrupt (with the `iret` instruction).