# INTRODUCTION

This folder contains all the rules relatives to the code of the project. It's a must to follow and understand all the rules and why it's important to follow them.

# Why it's important to follow a coding style ?

Following a coding style is really important because this is a public and communautary repository, you will not code alone, and you will not be the only one to read the code. So it's important to have a common style to make the code readable and understandable for everyone. Also, it's important to have a common style to make the code consistent and easy to maintain.

It's hard to accept but if your name is not one of the following: Linus Torvalds, Richard Stallman, Bjarne Stroustrup, Dennis Ritchie, Ken Thompson, Larry Wall, Guido van Rossum, you are **NOT** a god and you will not be able to write a perfect code. So it's important to follow a style to make the code readable and understandable for everyone and for detecting bugs.

# How to follow the coding style ?

All the coding style rules are in this folder and explains in the [RULES LIST](#rules-list) section. Just understand and follow them and you will be fine.

# Rules list <a name="rules-list"></a>

This is the list of all the rules that you have to follow to write a good code.

- [Indentation](indentation.md): You must use 4 spaces for indentation.
- [Line length](line_length.md): You must not exceed 100 characters per line, excepts for comments. Not a strict rule but it's better to not exceed more than 100 characters.
- [Comments](comments.md): **DO NOT** comments your code inside the functions, you can add an header comment to explain what the function does but not line by line. If it's necessarry please add a link into a mardkown file in the `doc` folder.
- [Naming](naming.md): You must follow the snake_case naming convention for variables and functions, macro must be in UPPER_SNAKE_CASE.
- [Headers](headers.md): You must add a header comment to all the functions, it must explains at least the parameters and the return value. If the prototype is in a header file, you must add a header comment to the header file at least. If the function is static, just add it above the function.
- [Braces](braces.md): You must use braces for all the control structures, even if it's not necessarry. All the braces must be on the same line as the control structure. Excepts for the functions, you must put the opening brace on the next line.
- [Spaces](spaces.md): You must use spaces around operators and after commas. You must not use spaces around parentheses.
- [Pointers](pointers.md): the pointer symbol (`*`) must be on the variable name, not on the type.
- [Types](types.md): You must use the proper types for the variables
- [Constants](constants.md): You must use the `const` keyword for constants datas.
- [Structures](structures.md): You must use the `typedef` keyword for structures.
- [Functions](functions.md): You must use the `static` keyword for functions that are not used outside of the file.
- [Global variables](global_variables.md): You must not use global variables, if you need to use a global variable, you must add a comment to explain why you need it.
- [Includes](includes.md): You must not include a header file in another header file, you must include the header file in the source file. Excepts when it's necessarry.
- [Guards](guards.md): You must use guards for all the header files and not `#pragma once`.
- [SRP](srp.md): You must follow the Single Responsibility Principle. A function must do only one thing, same for a file or a folder, it must contains only one subject.
- [DRY](dry.md): You must follow the Don't Repeat Yourself principle. If you need to repeat the same code, you must create a function or a macro to avoid the repetition.
