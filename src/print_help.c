#include "infiX.h"

/**
 * print_help - print help text
 * @err_type: a string specifying which text to print
 */
void print_help(const char *err_type)
{
	if (!_strcmp(err_type, "base"))
	{
		fprintf(stderr, "Only decimals (base 10) are currently supported.\n");
		fprintf(stderr, "-Any character that does not represent a number in ");
		fprintf(stderr, "the specified base will\n");
		fprintf(stderr, " cause the rest of the string to be ignored.\n");
	}
	else if (!_strcmp(err_type, "usage"))
	{
		fprintf(stderr, "USAGE: num1 operator [num2]\nFor operators requring ");
		fprintf(stderr, "two numbers, second number will be assumed as 0\n.");
		fprintf(stderr, "if not given.\n");
	}
	else if (!_strcmp(err_type, "operator"))
	{
		fprintf(stderr, "OPERATORS:\n'+': addition, '-': subtraction,");
		fprintf(stderr, " 'x': multiplication, '/': division,\n'%%': ");
		fprintf(stderr, "modulus.\n");
	}
	else
	{
		fprintf(stderr, "USAGE:\n");
		fprintf(stderr, "%s num1 operator [num2]\n", program_invocation_name);
		fprintf(stderr, "Operators: '+' '-' 'x'  '/' '%%'.\n");
		fprintf(stderr, "Only decimals (base 10) are currently supported.\n");
	}
}
