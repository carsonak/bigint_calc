#!/usr/bin/awk -field
# This script breaks long lines with a delimeter(delimeter) at every n intervals(step).
# Ideally it is used to break long numbers with commas and therefore reads lines
# from right to left.

# BUG: Duplicates some characters in the last field to if length of the line was not
# a multiple of `step`.

BEGIN {
    "mktemp /tmp/add_spaces.awk.XXXXXXXXX" | getline tempfile
    step = strtonum(step)
    if (step == 0) {
        step = 9
    }

    if (delimeter == "") {
        delimeter = ","
    }
}

{
    # Iterate over the fields in the current line
    for (field = 1; field <= NF; field++) {
        f_len = length($field)
        h = f_len
        modified_field = ""
        if (substr($field, f_len, 1) == "\n") {
            --h
            modified_field = "\n"
        }

        if (f_len <= step) {
            modified_field = $field FS modified_field
        }

        # Insert a delimeter every n step
        while (h > 0 && f_len > step) {
            h = h - step
            modified_field = substr($field, h, step) delimeter modified_field
        }

        modified_record[field] = modified_field
    }

    for (f = 1; f <= NF; f++) {
        printf modified_record[f] >> tempfile
    }

    print "" >> tempfile
}

END {
    # Overwrite the original file
    system("cat " tempfile " > " FILENAME)
    system("rm " tempfile)  # Remove the temporary file
}
