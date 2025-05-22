#!/usr/bin/bash
# Read numbers from a json file and convert them to parseable C arrays.

function str_to_c_array () {
    ./parse_json.py -f speed_test_nums.json "$1" | ./str_to_numlist.py | tr "[]" "{}"
}

function process_json_nums () {
    local start=$1
    local stop=$2
    local step=$3
    for (( i=start; i<stop; i+=step ))
        do local base_key="u$((stop * 10)).$((i * 9))"
        local array
        array=$(str_to_c_array "${base_key}a")
        printf "len=%d %s\n" "$(echo "$array" | tr " " "\n" | wc -l)" "$array"
        array=$(str_to_c_array "${base_key}b")
        printf "len=%d %s\n" "$(echo "$array" | tr " " "\n" | wc -l)" "$array"
    done
}

(
    process_json_nums 1 10 1 &&
    process_json_nums 10 100 10 &&
    process_json_nums 100 1000 100 &&
    process_json_nums 1000 10000 1000 # &&
    # process_json_nums 10000 50000 10000
) > "speed_test_nums_as_arrays.txt"
