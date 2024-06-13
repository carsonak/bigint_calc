#!/usr/bin/python
"""Convert a number string to a reversed array."""

from collections.abc import Iterable
import sys


if hasattr(sys, "set_int_max_str_digits"):
    sys.set_int_max_str_digits(200_000)


def numstr_to_numarray(input_strings: Iterable[str]) -> str:
    """Return the repr of a list of ints from a string of decimals."""
    output: str = ""
    for numstr in input_strings:
        numstr = numstr.strip()
        if not numstr or not numstr[0].isdigit() and len(numstr) < 2:
            output = "".join([output, numstr, "\n"])
            continue

        str_i: int = len(numstr)
        prev: int = str_i
        num_list: list[int] = []
        while str_i > 0:
            str_i = 0 if str_i - 9 < 0 else str_i - 9
            section: str = numstr[str_i:prev]
            if section == "-" and len(numstr) > 1:
                num_list[-1] *= -1
            else:
                num_list.append(int(section))

            prev = str_i

        output = "".join([output, str(num_list), "\n"])

    return output.strip()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        print(numstr_to_numarray(sys.argv[1:]))
    else:
        print(numstr_to_numarray(sys.stdin))
