#!/usr/bin/env python3
"""Convert a number in a string to a reversed list of integers."""

from collections.abc import Iterable
import sys


if hasattr(sys, "set_int_max_str_digits"):
    sys.set_int_max_str_digits(200_000)

BIGINT_BASE = 1_000_000_000


def numstr_to_numarray(input_strings: Iterable[str]) -> str:
    """Convert a number in a string to a reversed list of integers."""
    output: str = ""
    carry: int = 0
    for numstr in input_strings:
        is_negative: bool = False
        numstr = numstr.strip()
        if not numstr or (not numstr[0].isdigit() and len(numstr) == 1):
            output = "".join([output, numstr, "\n"])
            continue

        if numstr[0] == "-":
            is_negative = True
            numstr = numstr[1:]

        str_i: int = len(numstr)
        prev: int = str_i
        num_list: list[int] = []
        while str_i > 0:
            str_i = 0 if str_i - 9 < 0 else str_i - 9
            section: str = numstr[str_i:prev]
            carry += int(section)
            num_list.append(carry % BIGINT_BASE)
            carry //= BIGINT_BASE
            prev = str_i

        while carry:
            num_list.append(carry % BIGINT_BASE)
            carry //= BIGINT_BASE

        if is_negative:
            num_list[-1] *= -1

        output = "".join([output, str(num_list), "\n"])

    return output.strip()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        print(numstr_to_numarray(sys.argv[1:]))
    else:
        print(numstr_to_numarray(sys.stdin))
