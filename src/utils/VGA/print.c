#include "VGA.h"
#include "print.h"

int vga_putchar_at(char c, uint8_t color, uint8_t x, uint8_t y)
{
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return -1;
    if (color > 0x0F)
        return -2;
    const size_t index = (y * VGA_WIDTH + x) * 2;
    VGA_MEMORY[index] = c;
    VGA_MEMORY[index + 1] = color;
    return 0;
}
