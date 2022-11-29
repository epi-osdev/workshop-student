#include "timer.h"

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
