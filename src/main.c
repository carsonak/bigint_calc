/*!
 * @file
 * @brief bigint_calc.
 */

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>  // fileno

#include <errno.h>  // errno
#include <stddef.h>  // NULL
#include <stdio.h>   // FILE
#include <stdlib.h>  // EXIT_FAILURE
#include <string.h>  // strerror

#include "main.h"
#include "parser.h"
#include "reader.h"

/*!
 * @brief entry point.
 *
 * @param argc number of arguments.
 * @param argv pointer to an array of strings.
 *
 * @return 0 on success, 1 on error.
 */
int main(int argc, char *argv[])
{
	program context = {.invoke_name = argv[0]};
	reader r = {.stream = stdin, .prompt = "> "};

	if (argc > 1)
	{
		context.src_file = argv[1];
		r.stream = fopen(context.src_file, "r");
		if (!r.stream)
		{
			fprintf(
				stderr, "ERROR: Failed to open '%s': %s\n", context.src_file,
				strerror(errno)
			);

			return (EXIT_FAILURE);
		}
	}
	else if (isatty(fileno(stdin)))
	{
		r.stream = tmpfile();
		if (!r.stream)
		{
			fprintf(
				stderr, "ERROR: Failed to open temporary file: %s\n",
				strerror(errno)
			);

			return (EXIT_FAILURE);
		}
	}

	parse(&r);
	if (r.stream != stdin)
		fclose(r.stream);

	return (EXIT_SUCCESS);
}
