#include "parsing.h"

/**
 * get_token - tokenises characters in a string.
 * @str: the string.
 * @processed: address to store number of characters processed.
 */
static token *get_token(const char *str, size_t *processed)
{
	token *t = str ? malloc(sizeof(*t) * 1) : NULL;
	numstr *n = NULL;

	if (!t || !str)
		return (NULL);

	switch (*str)
	{
	case '(':
	{
		*t = (token){.id = PAREN_L, .op = {NULL}};
		break;
	}
	case ')':
	{
		*t = (token){.id = PAREN_R, .op = {NULL}};
		break;
	}
	case '+':
	{
		*t = (token){.id = ADD_OP, .op = {bn_addition}};
		break;
	}
	case '-':
	{
		*t = (token){.id = SUB_OP, .op = {bn_subtraction}};
		break;
	}
	case '*':
	{
		*t = (token){.id = MUL_OP, .op = {bn_multiplication}};
		break;
	}
	case '/':
	{
		*t = (token){.id = DIV_OP, .op = {bn_division}};
		break;
	}
	case '%':
	{
		*t = (token){.id = MOD_OP, .op = {bn_modulus}};
		break;
	}
	case '^':
	{
		*t = (token){.id = POW_OP, .op = {bn_power}};
		break;
	}
	default:
	{
		n = str_to_numstr(str, 10, processed);
		if (!n)
			t = free_n_null(t);
		else
			*t = (token){.id = NUMBER, .op.number = *n};

		free_n_null(n);
		return (t);
	}
	}

	if (processed)
		*processed = 1;

	return (t);
}

/**
 * is_binary_operator - checks if element is a binary operator.
 * @id: element id to check.
 *
 * Return: true if the element is a binary operator, false otherwise.
 */
static bool is_binary_operator(enum token_type id)
{
	return (id == ADD_OP || id == SUB_OP || id == MUL_OP ||
			id == DIV_OP || id == MOD_OP || id == POW_OP);
}

/**
 * is_unary_prefix_operator - checks if element is a unary prefix operator.
 * @id: element id to check.
 *
 * Return: true if the element is a unary prefix operator, false otherwise.
 */
static bool is_unary_prefix_operator(enum token_type id)
{
	return (id == ADD_OP || id == SUB_OP);
}

/**
 * is_starter - checks element can begin an expression.
 * @id: element id to check.
 *
 * Return: true if the element can begin an expression, false otherwise.
 */
static bool is_starter(enum token_type id)
{
	return (id == NUMBER || id == PAREN_L);
}

/**
 * is_ender - checks element can terminate an expression.
 * @id: element id to check.
 *
 * Return: true if the element can terminate an expression, false otherwise.
 */
static bool is_ender(enum token_type id)
{
	return (id == NUMBER || id == PAREN_R);
}

/**
 * syntax_is_valid - validates syntax by comparing adjacent tokens.
 * @left: left hand token.
 * @right: right hand token.
 *
 * Return: true if syntax is ok, otherwise false.
 */
static bool syntax_is_valid(token *left, token *right)
{
	if (!right)
		return (false);

	if (!left &&
		!is_starter(right->id) &&
		!is_unary_prefix_operator(right->id))
	{ /*Expression must begin with a: 'number', '(', '+' or '-'.*/
		return (false);
	}

	if (left && left->id == NUMBER && right->id == NUMBER)
	{ /*There cannot be two consecutive numbers.*/
		return (false);
	}

	if (left &&
		!is_unary_prefix_operator(left->id) &&
		is_binary_operator(left->id) &&
		!is_starter(right->id))
	{ /*Binary operators must be followed by a: 'number' or '('.*/
		return (false);
	}

	return (true);
}

/**
 * free_token - frees a token.
 * @freeable_token: pointer to a token struct.
 *
 * Return: NULL always.
 */
void *free_token(void *freeable_token)
{
	token *p = (token *)freeable_token;

	if (p && p->id == NUMBER)
		free_n_null(p->op.number.str);

	return (free_n_null(p));
}

/**
 * print_error - prints a parsing error.
 * @expression: pointer to whole expression.
 * @idx: index of the first character where error occurred.
 * @msg: message to print.
 */
void print_syntax_error(const char *expression, size_t idx, const char *msg)
{
	char slice[12] = {0}, highlight[12] = {0};
	int i = 0, upper = 0;

	if (!expression)
		return;

	memset(highlight, ' ', 11);
	upper = idx > 5 ? 5 : (int)idx;
	highlight[upper] = '^';
	for (i = 0; i < upper; i++)
		slice[i] = expression[idx - upper + i];

	for (i = 0; expression[idx + i] && i < 6; i++)
		slice[upper + i] = expression[idx + i];

	fprintf(stderr, "ParsingError:");
	if (msg)
		fprintf(stderr, "%s:", msg);

	fprintf(stderr, "\n...%s...\n   " ANSI_RED "%s" ANSI_NORMAL, slice, highlight);
}

/**
 * parser - converts an infix expression to an RPN deque of tokens.
 * @expression: pointer to a math expression.
 *
 * Return: pointer to a deque, NULL on failure.
 */
deque *parser(const char *expression)
{
	stack number_stack = {0}, operator_stack = {0};
	deque *output = NULL;
	token *current = NULL, *prev = NULL;
	size_t exp_i = 0, processed = 0;

	while (expression[exp_i] && expression[exp_i] != '\n')
	{
		while (expression[exp_i] &&
			   expression[exp_i] != '\n' &&
			   expression[exp_i] == ' ')
			exp_i++;

		prev = current;
		current = get_token(&expression[exp_i], &processed);
		if (!syntax_is_valid(prev, current))
		{
			print_syntax_error(expression, exp_i + processed, "invalid syntax");
			break;
		}
		/*Identifying unary prefix operators.*/
		if (!is_ender(prev->id) && is_unary_prefix_operator(current->id))
			current->op.operator= NULL;

		if (current->id == NUMBER)
		{
			if (!push(&number_stack, current))
				break;

			continue;
		}

		/*TODO: Finish this shunting yard.*/
		exp_i += processed;
	}

	while (operator_stack.size)
	{
		if (!push_head(output, pop(&operator_stack)))
			break;
	}

	if ((expression[exp_i] && expression[exp_i] != '\n') ||
		number_stack.size || operator_stack.size)
	{
		current = free_token(current);
		clear_stack(&number_stack, free_token);
		clear_stack(&operator_stack, free_token);
		output = clear_deque(output, free_token);
	}

	return (output);
}
