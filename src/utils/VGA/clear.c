#include "clear.h"
#include "VGA.h"

void vga_clear_screen()
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        VGA_MEMORY[i] = '0';
        VGA_MEMORY[i + 1] = 0x00;
    }
}
