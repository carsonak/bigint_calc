# INFIX MATH

Trying to write abitrary precision math functions in C.
The functions here are still quite simple and need more work.

**Debugging:** gcc -std=c17 -lm -lrt -g -pedantic -Wall -Werror -Wextra
-fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all
./\*.c -o ./infiX_math

**Release:** gcc -std=c17 -pedantic -Wall -Werror -Wextra -O3 ./\*.c -o
./infiX_mathO3 -lm -lrt

## Usage

As this is still in production not alot can be done. Currently only the basic
math operations are available.To try out one of the functions run this command
on a terminal:

./infiX_mathO3 *\<number\>* *\<sign\>* *\<number\>*

Where *sign* is one of: "+" "-" "/" "x"

## TODO

- Support for negative numbers
- Exponetials
- Square root
- Floating Points

## Functions

#### infix_add

Arbitrary precision addition function.

#### infix_mul

Arbitrary precision multiplication function.

#### infix_sub

Arbitrary precision subtraction function.

#### infix_div

Arbitrary precision division function.
