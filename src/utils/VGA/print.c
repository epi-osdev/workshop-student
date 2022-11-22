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

static void revstr(char *str) {
    char *end = str;
    char tmp;

    if (str) {
        while (*end) {
            ++end;
        }
        --end;
        while (str < end) {
            tmp = *str;
            *str++ = *end;
            *end-- = tmp;
        }
    }
}

static void itoa(int num, char *str, uint8_t base) {
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num /= base;
    }
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    revstr(str);
}

int vga_print_int_at(int num, uint8_t color, uint8_t x, uint8_t y)
{
    char str[16] = {0};

    itoa(num, str, 10);
    return vga_putstr_at(str, color, x, y);
}
