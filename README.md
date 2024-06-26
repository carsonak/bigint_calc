# infiX_math

This project aims to provide basic calculator functionality for numbers of
arbitrary length. This is a personal project meant to challenge myself and
just to practice C.

The [math](./math) executable was compiled with [gcc version 13.2.0](https://gcc.gnu.org/gcc-13/).

## Building from source

You can build the program by cloning the project into your own machine and running:

`make release`

## Usage

The executable can be run as a command line tool as follows:

`./math <number> '<operator>' <number>`

`<number>` is a decimal number not containing any spaces.

| **Operator** | **Description** |
| ------ | ------- |
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `//` | Division |
| `%` | Modulus |

Shell metacharacters such as `*` should be quoted.

## TODO

- Support for Floating Point numbers.
- Exponentiation.
- Support for longer expressions.
- Parenthesis.
- Provide proper interfaces for use as library and a command line tool.
- Allow underscores and commas as thousands separators.
- Support for hexadecimal numbers.
