#include "VGA.h"

void init()
{
    vga_clear_screen();
}

void kernel_main()
{
    init();
    vga_putchar_at('H', 0x0F, 0, 0);
    char str[] = "Hello World!";
    vga_putstr_at(str, 0x0F, 0, 1);
    vga_putchar_at('H', 0x0F, 0, 2);
}
