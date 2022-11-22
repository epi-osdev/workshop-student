#ifndef __EOS_VGA_PRINT_H__
    #define __EOS_VGA_PRINT_H__

    #include "types.h"

/**
* @brief Prints a string to the VGA buffer with the given color and position
* @param c: the char to print
* @param color: the color to print the char with
* @param x: the x position to print the char at
* @param y: the y position to print the char at
* @return 1 if success
* @return -1 if position error
*/
int vga_putchar_at(char c, uint8_t color, uint8_t x, uint8_t y);

/**
* @brief Prints a string to the VGA buffer with the given colors at the given position
* @param str: the string to print
* @param color: the color to print the string with
* @param x: the x position to print the string at
* @param y: the y position to print the string at
* @return length of the string printed if success
* @return -1 if position error
*/
int vga_putstr_at(const char *str, uint8_t color, uint8_t x, uint8_t y);

/**
* @brief Prints a number to the VGA buffer with the given colors at the given position
* @param num: the number to print
* @param color: the color to print the number with
* @param x: the x position to print the number at
* @param y: the y position to print the number at
* @return length of the number printed if success
* @return -1 if position error
*/
int vga_print_int_at(int num, uint8_t color, uint8_t x, uint8_t y);

/**
* @brief Prints a formatted string to the VGA buffer with the given colors at the given position
* @param fmt: the formatted string to print
* @param color: the color to print the formatted string with
* @param x: the x position to print the formatted string at
* @param y: the y position to print the formatted string at
* @param ...: the arguments to the formatted string
* @return length of the formatted string printed if success
* @return -1 if position error
*/
int vga_printf_at(const char *format, uint8_t color, uint8_t x, uint8_t y, ...);

#endif
