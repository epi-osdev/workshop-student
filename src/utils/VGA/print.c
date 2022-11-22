#include "VGA.h"
#include "print.h"

static size_t get_index(uint8_t x, uint8_t y)
{
    return y * 2 * VGA_WIDTH + x * 2;
}

int vga_putchar_at(char c, uint8_t color, uint8_t x, uint8_t y)
{
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return -1;
    const size_t index = get_index(x, y);
    VGA_MEMORY[index] = c;
    VGA_MEMORY[index + 1] = color;
    return 0;
}

int vga_putstr_at(const char *str, uint8_t color, uint8_t x, uint8_t y)
{
    size_t size_printed = 0;

    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return -1;
    for (size_t i = 0; str[i]; ++i, ++size_printed)
        vga_putchar_at(str[i], color, x + i, y);
    return size_printed;
}
