#!/usr/bin/env python3
"""Generate random numbers for speed tests."""

import json
from random import choices
import string
import sys


def main(ouput_file: str = "speed_test_nums.json") -> None:
    """Write random numbers of various sizes to a json file."""
    categories = {
        "u100": range(1, 10),
        "u1000": range(10, 100, 10),
        "u10000": range(100, 1000, 100),
        "u100000": range(1000, 10_000, 1000),
        "u500000": range(10_000, 50_000, 10_000)
    }
    numbers_dict: dict[str, dict[str, str]] = dict()
    for cat, digits_range in categories.items():
        numbers_dict[cat] = dict()
        for n in digits_range:
            numbers_dict[cat][f"{n*9}a"] = "".join(
                choices(string.digits, k=n*9))
            numbers_dict[cat][f"{n*9}b"] = "".join(
                choices(string.digits, k=n*9))

    with open(ouput_file, "w", encoding="utf-8") as file:
        json.dump(numbers_dict, file, indent=4)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        main()
