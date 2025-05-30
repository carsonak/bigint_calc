/*!
 * @file
 * @brief bigint_calc.
 */

#include <stdlib.h> /* NULL */

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
	char *answer = NULL;

	if (argc < 3)
	{
		panic((const char *)NULL);
		return (EXIT_FAILURE);
	}

	answer = parse_expression(argv[1], argv[2], argv[3]);
	if (!answer)
		return (EXIT_FAILURE);

	printf("%s\n", answer);
	answer = xfree(answer);
	return (EXIT_SUCCESS);
}
