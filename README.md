# INFIX MATH

Trying to write abitrary math functions in C.

Debugging: gcc -std=c17 -lm -lrt -g -pedantic -Wall -Werror -Wextra -fdiagnostics-color=always
-fsanitize=undefined -fsanitize-undefined-trap-on-error -fstack-protector-all ./\*.c -o ./infiX_math

Release: gcc -std=c17 -lm -lrt -pedantic -Wall -Werror -Wextra -O3 ./\*.c -o ./infiX_math
