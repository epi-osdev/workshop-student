#ifndef __EOS_UTILS_STRING_ITOA_H__
    #define __EOS_UTILS_STRING_ITOA_H__

    #include "types.h"

/**
* @brief This function converts an integer to a string and stores it in a buffer.
* @param num The number to be converted.
* @param str The buffer to store the string.
* @param base The base of the number.
*/
void itoa(int num, char *str, uint8_t base);

#endif