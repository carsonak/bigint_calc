# infiX_math

## Notice

This is a personal project meant to challenge myself and just to practice
writing C projects. Therefore this program and the suite of functions
thereof are not guaranteed to be up to production standards. Use at your
own risk.

## Description

This project aims to provide basic calculator functionality for numbers of arbitrary
length.
The [math](./math) executable was compiled with [gcc version 13.2.0](https://gcc.gnu.org/gcc-13/).

## Building from source

You can build the program by cloning the project into your own machine and running:

`make release`

## Usage

The executable can be run as a command line tool as follows:

**./math *number* '*operator*' *number***

| ***Operator*** | ***Description*** |
| ------ | ------- |
| **+** | Addition |
| **-** | Subtraction |
| **\*** | Multiplication |
| **%** | Modulus |

Shell metacharacters such as `*` should be quoted.

## TODO

- Support for Floating Point numbers.
- Exponentiation.
- Support for longer expressions.
- Parenthesis.
- Provide proper interfaces for use as library and a command line tool.
- Allow underscores and commas as thousands separators.
- Support for hexadecimal numbers.
