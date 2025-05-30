/*!
 * @file
 */

#include <stdio.h> /* fprintf */

#include "main.h"

/* ERRORS: */
/* "base" - unrecognised base */
/* "ops" - unrecognised operand */
/* "division by zero" - ... */
/* "unrecognised characters" - ... */

/*!
 * @brief app misuse error handler.
 *
 * @param err_type a string with the error type.
 */
void panic(char const *const err_type)
{
	if (!err_type || (err_type && err_type[0] == 'H'))
		help_me(err_type);
	else if (!strcmp(err_type, "invlaid chars"))
	{
		fprintf(stderr, "The string contains an invalid character. ");
		fprintf(stderr, "Valid characters [0-9].\n");
	}
	else if (!strcmp(err_type, "base"))
	{
		fprintf(stderr, "Only decimals (0-9) are currently supported.\n");
	}
	else if (!strcmp(err_type, "ops"))
	{
		fprintf(stderr, "OPERATORS:\n'+', '-', 'x', '/', '%%'\n");
	}
	else
	{
		fprintf(stderr, "USAGE: num1 operator [num2]\n");
	}
}

/*!
 * @brief print help text.
 *
 * @param which_help a string specifying which text to print.
 */
void help_me(char const *const which_help)
{
	if (which_help && !strcmp(which_help, "Hops"))
	{
		printf("OPERATORS:\n'+': addition, '-': subtraction,");
		printf(" 'x': multiplication, '/': division,\n'%%': ");
		printf("modulus.\n");
	}
	else if (which_help && !strcmp(which_help, "Hbase"))
	{
		fprintf(stderr, "Only decimals (0-9) are currently supported.\n");
		fprintf(stderr, "-Any character that does not represent a number in ");
		fprintf(stderr, "the specified base will\n");
		fprintf(stderr, " cause the rest of the string to be ignored.\n");
	}
	else if (which_help && !strcmp(which_help, "Husage"))
	{
		printf("USAGE:\n%s num1 operator [num2]\n", program_invocation_name);
		printf("For operators requring two numbers, second number will be ");
		printf("assumed as 0\n.if not given.\n");
	}
	else
	{
		printf("USAGE:\n");
		printf("%s num1 operator [num2]\n", program_invocation_name);
		printf("Operators: '+' '-' 'x'  '/' '%%'.\n");
		printf("Only decimals (base 10) are currently supported.\n");
	}
}
