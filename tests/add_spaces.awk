#!/usr/bin/awk -f

BEGIN {
    tempfile = system("mktemp /tmp/add_spaces.awk.XXXXXXXXX > /dev/null")
}

{
    # Read all lines in the file
    g = 1
    for (i = length($0); i >= 1; i--) {
        printf "%s", substr($0, i, 1) >> tempfile
        if (g % 9 == 0 && i - 1 > 0 && substr($0, i - 1, 1) != " ") {
            printf " " >> tempfile
        }

        g++
    }

    printf "\n\n" >> tempfile
}

END {
    # Reverse the temporary file and overwrite the original file
    system("rev " tempfile " > " FILENAME)
    system("rm " tempfile)  # Remove the temporary file
}
