# Calc
## Introduction
This library provides an easy and effecient way to calculate the result of an
expression. E.g. this line prints 7
```printf("%f\n", calc("1 + 2 * 3", NULL, NULL, NULL));```

## Features
- Operators: <tt>+ - * /</tt>
- Functions
- Variables
- Error Handling

## Building
The library can be built using CMake. It can only be built as a static library.
If *CALC_BUILD_EXAMPLE* is enabled the example is included.

## Installation
This library provides a CMake file which can be used to build it and install it
on your system. It can be included in any CMake project with
<tt>find_package(Calc)</tt> after installing it. You can however also copy the
files into your project.

## Usage
See [Calc.h (Doxygen)](https://www.preinfalk.co.at/projects/Calc/_calc_8h.html)
