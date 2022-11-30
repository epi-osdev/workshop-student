# INTRODUCTION <a name="introduction"></a>

This file explains the code in the [timer.c](../../../src/interrupts/timer.c) file.
The timer is a hardware device that sends an interrupt to the CPU at a regular interval. It's used to schedule tasks and to create a time reference. It can be usefull for us to create a time reference for things like a sleep function. or to schedule tasks in general.

# TABLE OF CONTENTS

- [INTRODUCTION](#introduction)
- [CODE EXPLANATION](#code-explanation)

# CODE EXPLANATION <a name="code-explanation"></a>

This file for now only contains this:
```c
static unsigned int tick = 0;

static void timer_callback(__attribute__((unused))registers_t *regs)
{
    tick++;
}

void init_timer(unsigned int freq)
{
    isr_register_handler(32, timer_callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);

    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
```
We have two functions:
- One for the initialization of the timer
- One for the callback of the timer

The timer is called each time by the interrupts `32`. We can set a frequency for the timer. The frequency is the number of times the timer is called per second. The higher the frequency, the more precise the time reference will be. But the higher the frequency, the more CPU time it will take to call the timer. So we have to find a balance between the two.
To set the frequency we have to set the divisor of the timer. The divisor is the number of ticks per second. The higher the divisor, the lower the frequency. The lower the divisor, the higher the frequency. The formula to calculate the divisor is `1193180 / freq`. `1193180` is the base frequency of the timer. `freq` is the frequency we want to set. The result of the formula is the divisor. We have to split the divisor in two parts. The lower part and the higher part. The lower part is the first 8 bits of the divisor. The higher part is the last 8 bits of the divisor. We have to send the lower part to the port `0x40` and the higher part to the port `0x40` too. But before sending the lower part we have to send the command `0x36` to the port `0x43`. This command is used to set the divisor of the timer. The lower part is sent first and then the higher part. The order is important.

The `timer_callback` function is just a function that increments the static `tick` variable. This variable is used to create a time reference. We can use this variable to create a sleep function for example.
