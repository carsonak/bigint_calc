#!/usr/bin/env python3
"""Encode a number in different bases."""

import string


def num_to_anybase(num: int, base: int) -> list[int]:
    """Encode a number to a list to any base greater than 1."""
    if base < 2:
        raise ValueError("base cannot be less than 2")

    if num == 0:
        return [0]

    digits: list[int] = []
    while num:
        num, i = divmod(num, base)
        digits.append(i)

    digits.reverse()
    return digits


def num_to_base62(num: int, base: int) -> str:
    """Encode a number to a string in any base in the range 2-62."""
    if base < 2:
        raise ValueError("base cannot be less than 2")

    if base > 62:
        raise ValueError("maximum base is 62")

    symbols = string.digits + string.ascii_uppercase + string.ascii_lowercase
    return ("-" if num < 0 else "" +
            "".join([symbols[digit] for digit in num_to_anybase(num, base)]))
