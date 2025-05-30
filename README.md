# bigint_calc

This is a command line program that behaves as a calculator, evaluating simple
mathematical expresions on numbers of arbitrary precision while relying solely
on C standard libraries.

## Building the executable

To build the executable you will need [CMake](https://cmake.org/download/).
While in the project root directory run the following command in a terminal to setup the build system.

`cmake -S . -B build`

Then run the following commands to build the executable and run tests.

`cmake --build build`

`ctest --test-dir build`

## Usage

| **Operator** | **Description** |
| ------ | ------- |
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `^` | Exponentiation |
| `/` | Division |
| `%` | Modulus |

Metacharacters such as `*` should be quoted to avoid accidental interpretation by the shell.

## TODO

- Support for Floating Point numbers.
- Support for longer expressions.
