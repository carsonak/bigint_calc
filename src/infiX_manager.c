#include "infiX.h"

#ifdef __cplusplus
extern "C"
{
#endif
	static math_func *get_math_func(char *op_symbol);
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
	m_uint *num1_arr = NULL, *num2_arr = NULL, *ans_arr = NULL;
	s_uchar *answer = NULL;
	math_func *func_ptr = NULL;

	if (!num1 || !op_symbol)
	{ /*Mandatory arguments missings*/
		panic("usage");
		return (NULL);
	}

	func_ptr = get_math_func(op_symbol);
	if (func_ptr)
	{
		errno = 0;
		/*Convert num1 and num2 to m_uint arrays first*/
		num1_arr = str_to_intarray((s_uchar *)num1);
		if (num1_arr)
			num2_arr = str_to_intarray((s_uchar *)num2);

		if (num1_arr && num2_arr)
			ans_arr = func_ptr(num1_arr, num2_arr);
	}

	free(num1_arr);
	free(num2_arr);
	if (ans_arr)
	{
		if (!_strcmp(op_symbol, "%"))
			answer = intarr_to_str(remain);
		else
			answer = intarr_to_str(ans_arr);
	}

	free(remain);
	free(ans_arr);
	if (!func_ptr)
		panic("ops"); /*Symbol not found*/

	return ((char *)answer);
}

/**
 * get_math_func - return the function associated with the give operator
 * @op_symbol: the operator symbol
 *
 * Return: pointer to a function on success, NULL on failure
 */
math_func *get_math_func(char *op_symbol)
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
		if (!_strcmp(operators[i].symbol, op_symbol))
			return (operators[i].f);

	return (NULL);
}
