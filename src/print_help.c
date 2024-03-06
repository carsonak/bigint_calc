#include "infiX.h"

/**
 * print_help - print help text
 * @err_type: a string specifying which text to print
 */
void print_help(const char *err_type)
{
	if (!_strcmp(err_type, "base"))
		fprintf(stderr, "Only decimals (base 10) are currently supported.\n");
	else if (!_strcmp(err_type, "usage"))
	{
		fprintf(stderr, "USAGE: num1 operand [num2]\nFor operators requring ");
		fprintf(stderr, "two numbers, second number will be assumed as 0\n.");
		fprintf(stderr, "if not given.\n");
	}
	else if (!_strcmp(err_type, "operator"))
	{
		fprintf(stderr, "Operators:\n'+': addition, '-': subtraction,");
		fprintf(stderr, " 'x': multiplication, '/': division.\n");
	}
	else
	{
		fprintf(stderr, "USAGE: num1 operand [num2]\n");
		fprintf(stderr, "Only decimals (base 10) are currently supported.\n");
		fprintf(stderr, "Operators: '+' '-' 'x'  '/'.\n");
	}
}
