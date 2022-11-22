#include "VGA.h"
#include "print.h"
#include "string.h"
#include <stdarg.h>

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
    return 1;
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

int vga_print_int_at(int num, uint8_t color, uint8_t x, uint8_t y)
{
    char str[16] = {0};

    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return -1;
    itoa(num, str, 10);
    return vga_putstr_at(str, color, x, y);
}

int vga_printf_at(const char *format, uint8_t color, uint8_t x, uint8_t y, ...)
{
    va_list args;
    int size_printed = 0;

    if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
        return -1;
    va_start(args, y);
    for (size_t i = 0; format[i]; ++i) {
        if (format[i] == '%') {
            switch (format[++i]) {
            case 'd':
                size_printed += vga_print_int_at(va_arg(args, int), color, x + size_printed, y);
                break;
            case 's':
                size_printed += vga_putstr_at(va_arg(args, char *), color, x + size_printed, y);
                break;
            case 'c':
                size_printed += vga_putchar_at(va_arg(args, int), color, x + size_printed, y);
                break;
            default:
                size_printed += vga_putchar_at(format[i], color, x + size_printed, y);
                break;
            }
        } else {
            size_printed += vga_putchar_at(format[i], color, x + size_printed, y);
        }
    }
    va_end(args);
    return size_printed;
}
