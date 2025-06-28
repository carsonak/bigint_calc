/*!
 * @file
 * @brief bigint_calc.
 */

#include <stddef.h>  // NULL
#include <stdlib.h>  // EXIT_FAILURE

#include "main.h"

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
	const char *const restrict program_name = argv[0];
	const char *restrict src_file = NULL;
	FILE *stream = stdin;

	if (argc > 1)
	{
		src_file = argv[1];
		stream = fopen(src_file, "r");
	}

	if (src_file)
		fclose(stream);

	return (EXIT_SUCCESS);
}
