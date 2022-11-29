#include "VGA.h"

void init()
{
    vga_clear_screen();
}

void kernel_main()
{
    init();
    char str[] = "Hello, World!";
    vga_printf_at(str, 0x0F, 0, 0);
}
