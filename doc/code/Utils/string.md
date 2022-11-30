# INTRODUCTION

String utils is a library that contains all the functions to manipulate strings. It's just basic functions as you may know, but it's a good start to manipulate strings and it will be useful in the future.

# TABLE OF CONTENTS

- [How it works ?](#how-it-works)
- [How to use it ?](#how-to-use-it)
- [Functions list](#functions-list)
- [Want to contribute ?](#want-to-contribute)

# How it works ? <a name="how-it-works"></a>

This is containing all the functions to manipulate strings. It's just basic functions as you may know, but it's a good start to manipulate strings and it will be useful in the future. All the functions are stored in the `src/utils/string/` folder.

# How to use it ? <a name="how-to-use-it"></a>

The string utils is a library that contains all the functions to manipulate strings. It's just basic functions as you may know, but it's a good start to manipulate strings and it will be useful in the future. It's providing an API named [string.h](../../../src/utils/string.h) that contains all the functions. Only this file must be included to use the string utils.

# Functions list <a name="functions-list"></a>

- [itoa](#func-itoa)
- [revstr](#func-revstr)

## itoa <a name="func-itoa"></a>

This function convert an integer to a string into the given base. It will return nothing but the final string will be stored in the buffer given in parameter. The buffer must be big enough to store the string.

Here is the prototype of the function:

```c
void itoa(int num, char *str, uint8_t base);
```

## revstr <a name="func-revstr"></a>

This function reverse a string. It will return nothing but the final string will be stored in the buffer given in parameter.

Here is the prototype of the function:

```c
void revstr(char *str);
```

# Want to contribute ? <a name="want-to-contribute"></a>

If you want to contribute to this utils, you can add your own function, please make sure that the code is clean and that it's working. If you want to add a function, please add it in the [string.h](../../../src/utils/string.h) file and add the documentation in the [string.md](string.md) file.
