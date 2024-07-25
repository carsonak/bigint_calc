#include "text_processing.h"

static math_function *get_math_function(const char *op_symbol);

/**
 * parse_expression - process a mathematical expression.
 * @tokens: number of tokens in the token array.
 * @token_array: an array of tokens to process.
 *
 * Return: Pointer to result numstring on success, NULL on failure.
 */
numstr *parse_expression(size_t tokens, const char *const *token_array)
{
	numstr *answer = NULL;
	size_t i = 0;

	for (i = 0; i < tokens; i++)
		get_math_function(token_array[i]);

	return (answer);
}

/**
 * get_math_function - return the function associated with the give operator.
 * @op_symbol: the operator symbol.
 *
 * Return: pointer to a function on success, NULL on failure.
 */
math_function *get_math_function(const char *op_symbol)
{
	int i = 0;
	op_func ops[] = {
		{.symbol = "^", .f = bn_power},
		{.symbol = "*", .f = bn_multiplication},
		{.symbol = "%", .f = bn_modulus},
		{.symbol = "/", .f = bn_division},
		{.symbol = "+", .f = bn_addition},
		{.symbol = "-", .f = bn_subtraction},
		{NULL, NULL},
	};

	for (i = 0; op_symbol && ops[i].symbol; i++)
		if (!strncmp(ops[i].symbol, op_symbol, strlen(ops[i].symbol) + 1))
			return (ops[i].f);

	return (NULL);
}
