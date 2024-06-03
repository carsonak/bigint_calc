#!/usr/bin/python
"""Operate on numbers from json document."""

import json
import sys
from typing import Callable, NamedTuple, TypeVar


class AccessKeys(NamedTuple):
    """Keys to extract numbers from a dict of dicts of str."""

    category: str
    key: str


_IORS = TypeVar("_IORS", bound=int | str)

if sys.version_info.major > 2 and sys.version_info.minor > 10:
    sys.set_int_max_str_digits(200_000)


def get_value(d: dict[str, dict[str, str]], k: AccessKeys) -> str:
    """Extract a value from a dict of dict."""
    return d[k.category][k.key]


def get_accesskeys(token: str, delimeter: str = ".") -> AccessKeys:
    """Extract accesskey from a token."""
    return AccessKeys(*token.split(delimeter))


def parse_argv(argv: list[str]) -> tuple[str, str | None, str | None] | None:
    """Process argv into operation to be executed."""
    if not 2 <= len(argv) <= 4:
        return None

    num1_token = argv[1]
    op: str | None = argv[2] if len(argv) > 2 else None
    num2_token = argv[3] if len(argv) > 3 else None
    return num1_token, op, num2_token


def main(argv: list[str]) -> None:
    """Entry point."""
    operators: dict[str, Callable[[_IORS, _IORS], int]] = {
        "+": lambda x, y: int(x) + int(y),
        "-": lambda x, y: int(x) - int(y),
        "*": lambda x, y: int(x) * int(y),
        "/": lambda x, y: int(int(x) / int(y)),
        "**": lambda x, y: int(x) ** int(y),
    }
    usage_message: str = (
        f"USAGE: {argv[0]} 'category_name.number_key' "
        f"[ '{"|".join(operators)}' "  # noqa: B907
        "[ 'category_name.number_key' ] ]"
    )

    tokens = parse_argv(argv)
    if tokens is None:
        print(usage_message, file=sys.stderr)
        return

    num1_keys: AccessKeys = get_accesskeys(tokens[0])
    op: Callable[[_IORS, _IORS], int] | None = None
    if tokens[1] is not None:
        op = operators.get(tokens[1])
        if op is None:
            print(usage_message, file=sys.stderr)
            return

    num2_keys: AccessKeys | None = None
    if tokens[2] is not None:
        num2_keys = get_accesskeys(tokens[2])

    if op is not None and num2_keys is None:
        print(usage_message, file=sys.stderr)
        return

    with open("large_numbers_as_str.json", encoding="utf-8") as f:
        numstr_dict: dict[str, dict[str, str]] = json.load(f)

    n1 = get_value(numstr_dict, num1_keys)
    n2 = "0"
    if num2_keys is not None:
        n2 = get_value(numstr_dict, num2_keys)

    if op is not None:
        print(f"{n1} {tokens[1]} {n2} = {op(n1, n2)}")
    else:
        print(n1)


if __name__ == "__main__":
    main(sys.argv)
