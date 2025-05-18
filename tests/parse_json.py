#!/usr/bin/env python
"""Operate on numbers from json document."""

from argparse import ArgumentParser
import json
import sys
from typing import Callable, NamedTuple

if hasattr(sys, "set_int_max_str_digits"):
    sys.set_int_max_str_digits(200_000)


class AccessKeys(NamedTuple):
    """Keys to extract numbers from a dict of dicts of str."""
    category: str
    key: str


def get_value(d: dict[str, dict[str, str]], k: AccessKeys) -> str:
    """Extract a value from a dict of dict."""
    return d[k.category][k.key]


def get_accesskeys(token: str, delimeter: str = ".") -> AccessKeys:
    """Extract accesskey from a token."""
    return AccessKeys(*token.split(delimeter))


def parse_argv(argv: list[str]
               ) -> tuple[str, AccessKeys, str | None, AccessKeys | None]:
    """Process argv into operation to be executed."""
    parser = ArgumentParser(
        description="Perform some simple math operations on "
        "numbers from json file."
    )
    parser.add_argument(
        "-f", "--json-file", default="nums_as_str.json",
        help="the json file to read from"
    )
    parser.add_argument("num1_token", metavar="category_name.number_key",
                        help="a string identifying a number in the json file")
    parser.add_argument(
        "operator", choices=["+", "-", "*", "/", "%", "**"], nargs="?",
        help=("optional symbol representing a mathematical operation "
              "to carry out")
    )
    parser.add_argument(
        "num2_token", metavar="category_name.number_key", nargs="?",
        help="an optional string identifying a number in the json file"
    )

    tokens = parser.parse_args(argv)
    num1_keys: AccessKeys = get_accesskeys(tokens.num1_token)

    operator: str | None = tokens.operator

    num2_keys: AccessKeys | None = None
    if tokens.num2_token is not None:
        num2_keys = get_accesskeys(tokens.num2_token)

    if (
        operator is None and num2_keys is not None or
        operator is not None and num2_keys is None
    ):
        parser.error(
            "invalid syntax: the operator and "
            "second number key are mutually inclusive"
        )

    return tokens.json_file, num1_keys, operator, num2_keys


def main(argv: list[str]) -> None:
    """Entry point."""
    ops: dict[str, Callable[[int | str, int | str], int]] = {
        "+": lambda x, y: int(x) + int(y),
        "-": lambda x, y: int(x) - int(y),
        "*": lambda x, y: int(x) * int(y),
        "/": lambda x, y: int(x) // int(y),
        "%": lambda x, y: int(x) % int(y),
        "**": lambda x, y: int(x) ** int(y),
    }

    json_file, num1_keys, operator, num2_keys = parse_argv(argv)
    with open(json_file, encoding="utf-8") as f:
        numstr_dict: dict[str, dict[str, str]] = json.load(f)

    n1 = get_value(numstr_dict, num1_keys)
    n2 = "0"
    if num2_keys is not None:
        n2 = get_value(numstr_dict, num2_keys)

    if operator is not None:
        print(f"{n1} '{operator}' {n2} =", ops[operator](n1, n2))  # noqa: B907
    else:
        print(n1)


if __name__ == "__main__":
    main(sys.argv[1:])
