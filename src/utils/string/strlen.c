#include "strlen.h"

size_t strlen(const char *str)
{
    if (str == NULL) {
        return 0;
    }
    size_t i;
    for (i = 0; str[i] != '\0'; i++);
    return i;
}
