#include "infiX.h"

/**
 * infiX_dealer - determine what operation to carry out based on given symbol
 * @num1: first number
 * @op_symbol: operand
 * @num2: second number
 *
 * Return: 1 on failure, 0 on success
 */
int infiX_dealer(char *num1, char *op_symbol, char *num2)
{
	int i = 0;
	mid_uint *num1_arr = NULL, *num2_arr = NULL, *ans_arr = NULL;
	lo_uchar *answer = NULL;
	op_func operators[] = {
		{"+", infiX_addition},
		{"-", infiX_subtraction},
		{"x", infiX_multiplication},
		{"/", infiX_division},
		{NULL, NULL},
	};

	if (!num1 || !num2 || !op_symbol)
	{
		print_help();
		return (EXIT_FAILURE);
	}

	for (i = 0; operators[i].symbol; i++)
	{
		if (operators[i].symbol[0] == op_symbol[0])
		{
			/*Convert num1 and num2 to mid_uint arrays first*/
			num1_arr = str_to_intarray((lo_uchar *)num1);
			if (!num1_arr)
				break;

			num2_arr = str_to_intarray((lo_uchar *)num2);
			if (!num2_arr)
				break;

			errno = 0;
			ans_arr = operators[i].f(num1_arr, num2_arr);
			break;
		}
	}

	free(num1_arr);
	free(num2_arr);
	free(remain);
	if (ans_arr)
	{
		answer = intarr_to_str(ans_arr);
		if (answer)
		{
			free(ans_arr);
			printf("%s\n", (char *)&answer[pad_char((char *)answer, "0")]);
			free(answer);
			return (EXIT_SUCCESS);
		}
	}

	if (!operators[i].symbol)
		print_help();

	return (EXIT_FAILURE);
}

/**
 * print_help - print help text
 */
void print_help(void)
{
	fprintf(stderr, "USAGE: <num1> <operand> <num2>\n");
	fprintf(stderr, "Only base 10 numbers are currently supported.\n");
	fprintf(stderr, "Operands: '+' '-' 'x'  '/'.\n");
}
