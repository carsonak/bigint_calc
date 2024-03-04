#include "infix.h"

/**
 * infiX_op - a wrapper function for the infiX_math functions
 * @num1: first number
 * @sign: operand
 * @num2: second number
 *
 * Return: 1 on failure, 0 on success
 */
int infiX_op(char *num1, char *sign, char *num2)
{
	size_t i = 0;
	mid_int *n1_arr = NULL, *n2_arr = NULL, *ans_arr = NULL;
	lo_int *answer = NULL;
	op_func ops[] = {
		{"+", infiX_add},
		{"-", infiX_sub},
		{"x", infiX_mul},
		{"/", infiX_div},
		{NULL, NULL},
	};

	if (!num1 || !num2 || !sign)
	{
		print_help();
		return (EXIT_FAILURE);
	}

	for (i = 0; ops[i].sign; i++)
	{
		errno = 0;
		if (ops[i].sign[0] == sign[0])
		{
			/**
			 * Send num1 and num2 to the converters first
			 * Offset pointers by number of leading zeros
			 */
			if (num1)
			{
				n1_arr = str_u32((lo_int *)&num1[pad_char(num1, "0")]);
				if (!n1_arr)
					return (EXIT_FAILURE);
			}

			if (num2)
			{
				n2_arr = str_u32((lo_int *)&num2[pad_char(num2, "0")]);
				if (!n2_arr)
				{
					if (n1_arr)
						free(n1_arr);

					return (EXIT_FAILURE);
				}
			}

			ans_arr = ops[i].f(n1_arr, n2_arr);
			break;
		}
	}

	if (n1_arr)
		free(n1_arr);

	if (n2_arr)
		free(n2_arr);

	if (remain)
		free(remain);

	if (ans_arr)
	{
		answer = u32_str(ans_arr);
		free(ans_arr);
		if (answer)
			printf("%s\n", (char *)&answer[pad_char((char *)answer, "0")]);
		else
			return (EXIT_FAILURE);

		free(answer);
		return (EXIT_SUCCESS);
	}
	else if (!ops[i].sign)
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
	fprintf(stderr, "Currently supported operands: '+' '-' 'x'  '/'.\n");
}
