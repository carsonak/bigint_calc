#include "infiX.h"

/**
 * main - entry point
 * @argc: number of arguments
 * @argv: pointer to an array of strings
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char *argv[])
{
	char *answer = NULL;

	errno = 0;
	if (argc < 3)
	{
		panic((const char *)NULL);
		return (EXIT_FAILURE);
	}

	answer = infiX_manager(argv[1], argv[2], argv[3]);
	if (!answer)
		return (EXIT_FAILURE);

	printf("%s\n", answer);
	free(answer);
	return (EXIT_SUCCESS);
}
