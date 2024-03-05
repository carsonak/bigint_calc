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
	errno = 0;
	if (argc != 4 || strlen(argv[2]) != 1)
	{
		print_help();
		return (EXIT_FAILURE);
	}
	else if (!isdigit(argv[1][0]) || !isdigit(argv[3][0]))
	{
		fprintf(stderr, "Insufficient digits\n");
		return (EXIT_FAILURE);
	}

	return (infiX_op(argv[1], argv[2], argv[3]));
}
