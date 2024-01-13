BEGIN {
    filename = "./bc_out"  # Replace with your actual file name
    tempfile = "temp_file.txt"

    getline file_contents < filename
    reversed_contents = file_contents

    # Process the reversed contents and write to a temporary file
    g = 1
    for (i = length(reversed_contents); i >= 1; i--) {
        printf "%s", substr(reversed_contents, i, 1) >> tempfile
        if (g % 9 == 0 && substr(reversed_contents, i - 1, 1) != " ") {
            printf " " >> tempfile
        }
        g++
    }
    printf "\n" >> tempfile

    # Reverse the temporary file and overwrite the original file
    system("rev " tempfile " > " filename)
    system("rm " tempfile)  # Remove the temporary file
}
