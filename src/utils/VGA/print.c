#include "VGA.h"
#include "print.h"

static int basic_pos_and_color_err(uint8_t color, uint8_t x, uint8_t y)
{
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return -1;
    if (color > 0x0F)
        return -2;
    return 0;
}

static size_t get_index(uint8_t x, uint8_t y)
{
    return y * 2 * VGA_WIDTH + x * 2;
}

int vga_putchar_at(char c, uint8_t color, uint8_t x, uint8_t y)
{
    int err = basic_pos_and_color_err(color, x, y);

    if (err != 0)
        return err;
    const size_t index = get_index(x, y);
    VGA_MEMORY[index] = c;
    VGA_MEMORY[index + 1] = color;
    return 0;
}

int vga_putstr_at(const char *str, uint8_t color, uint8_t x, uint8_t y)
{
    int err = basic_pos_and_color_err(color, x, y);

    if (err != 0)
        return err;
    for (size_t i = 0; str[i]; ++i)
        vga_putchar_at(str[i], color, x + i, y);
    return 0;
}
