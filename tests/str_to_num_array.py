#!/usr/bin/python
"""Convert a number string to a reversed array."""

from collections.abc import Iterable
import sys


if sys.version_info.major > 2 and sys.version_info.minor > 10:
    sys.set_int_max_str_digits(200_000)


def main(input_strings: Iterable[str]) -> None:
    """Entry point."""
    for numstr in input_strings:
        numstr = numstr.strip()
        if not numstr[0].isdigit() and not numstr.startswith("-"):
            print(numstr)
            continue

        str_i: int = len(numstr)
        prev: int = str_i
        num_list: list[int] = []
        while str_i > 0:
            str_i = 0 if str_i - 9 < 0 else str_i - 9
            section: str = numstr[str_i:prev]
            if section == "-":
                num_list[-1] *= -1
            else:
                num_list.append(int(section))

            prev = str_i

        print(num_list)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1:])
    else:
        main(sys.stdin)
