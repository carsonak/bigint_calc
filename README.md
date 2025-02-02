# bigint_calc

This is a command line program that behaves as a calculator, evaluating simple
mathematical expresions on numbers of arbitrary precision while relying solely
on C standard libraries.

## Usage

Run the [**math**](./math) executable on a terminal as follows:

`./math <number> '<operator>' <number>`

| **Operator** | **Description** |
| ------ | ------- |
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `^` | Exponentiation |
| `//` | Division |
| `%` | Modulus |

Metacharacters such as `*` should be quoted to avoid accidental interpretation by the shell.

## Building from source

### Pre-requisites

- GNU Compiler Collection a.k.a [gcc](https://gcc.gnu.org/install/).
- [GNU make](https://www.gnu.org/software/make/).

Once the pre-requisites have been met run the following command in a terminal
to build the executable.

`make release`

## TODO

- Support for Floating Point numbers.
- Support for longer expressions.
- Parenthesis.
- Improve speed and memory usage.
