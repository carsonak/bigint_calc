#!/usr/bin/gawk -f

BEGIN {
  tempfile = "/tmp/aggregateErrorsData.txt"
  print "=================== ERRORS =====================\n" > tempfile
}

{
  # Use an associative array to store counts for each line
  count[$0]++
}

END {
  # Print the count and the unique lines
  for (line in count) {
      printf "[% 4d cases] %s\n", count[line], line >> tempfile
  }

  system("cat " tempfile)
  system("rm " tempfile)
}
