#!/usr/bin/gawk -f

BEGIN {
}

{
  # Use an associative array to store counts for each line
  count[$0]++
}

END {
  temp = "tempfile.txt"
  print "=================== ERRORS =====================" > temp
  # Print the count and the unique lines
  for (line in count) {
      printf "[% 4d cases] %s\n", count[line], line >> temp
  }

  system("cat " temp)
  system("rm " temp)
}
