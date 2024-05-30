#include "infiX.h"

#ifdef __cplusplus
extern "C"
{
#endif
	static math_function *get_math_function(char *op_symbol);
#ifdef __cplusplus
}
#endif

/**
 * infiX_manager - determine what operation to carry out based on given symbol
 * @num1: first number
 * @op_symbol: operand
 * @num2: second number
 *
 * Return: Pointer to answer string on sucess, NULL on failure
 */
char *infiX_manager(char *num1, char *op_symbol, char *num2)
{
	uint32_t *num1_arr = NULL, *num2_arr = NULL, *ans_arr = NULL;
	char *answer = NULL;
	math_function *func_ptr = NULL;

	if (!num1 || !op_symbol)
	{ /*Mandatory arguments missings*/
		panic("usage");
		return (NULL);
	}

	func_ptr = get_math_function(op_symbol);
	if (func_ptr)
	{
		errno = 0;
		/*Convert num1 and num2 to uint32_t arrays first*/
		num1_arr = str_to_intarray(num1);
		if (num1_arr)
			num2_arr = str_to_intarray(num2);

		if (num1_arr && num2_arr)
			ans_arr = func_ptr(num1_arr, num2_arr);
	}

	free(num1_arr);
	free(num2_arr);
	if (ans_arr)
	{
		if (!strcmp(op_symbol, "%"))
			answer = intarr_to_str(remains);
		else
			answer = intarr_to_str(ans_arr);
	}

	free(remains);
	free(ans_arr);
	if (!func_ptr)
		panic("ops"); /*Symbol not found*/

	return (answer);
}

/**
 * get_math_function - return the function associated with the give operator
 * @op_symbol: the operator symbol
 *
 * Return: pointer to a function on success, NULL on failure
 */
math_function *get_math_function(char *op_symbol)
{
	int i = 0;
	op_func operators[] = {
		{"+", infiX_addition},
		{"-", infiX_subtraction},
		{"x", infiX_multiplication},
		{"/", infiX_division},
		{"%", infiX_division},
		{NULL, NULL},
	};

	for (i = 0; op_symbol && operators[i].symbol; i++)
		if (!strcmp(operators[i].symbol, op_symbol))
			return (operators[i].f);

	return (NULL);
}
