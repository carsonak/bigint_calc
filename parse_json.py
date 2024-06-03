#!/usr/bin/python
""""""

import json

with open("large_numbers_as_str.json", encoding="utf-8") as f:
    numstr_dict: dict[str, dict[str, str]] = json.load(f)

numlist_dict: dict[str, dict[str, list[int]]] = dict().fromkeys(numstr_dict)
for category, d in numstr_dict.items():
    numlist_dict[category] = dict().fromkeys(d)
    for k, numstr in d.items():
        i: int = len(numstr)
        prev: int = i
        numlist_dict[category][k] = []
        while i > 0:
            i = 0 if i - 9 < 0 else i - 9
            section: str = numstr[i:prev]
            if section == "-":
                numlist_dict[category][k][-1] *= -1
            else:
                numlist_dict[category][k].append(int(section))

            prev = i

with open("large_numbers_as_lists.json", "w", encoding="utf-8") as f:
    json.dump(numlist_dict, f, indent="    ")
