#include "revstr.h"

void revstr(char *str)
{
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
