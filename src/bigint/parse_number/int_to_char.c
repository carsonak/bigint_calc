#ifndef BIGINT_INT_TO_CHAR
#define BIGINT_INT_TO_CHAR

static char int_to_char(const unsigned short int num);

/**
 * int_to_char - map a decimal number in the range 0-35 to an ascii symbol for base36.
 * @num: the number to convert.
 *
 * Return: ascii symbol of the number, '\0' on error.
 */
static char int_to_char(const unsigned short int num)
{
	if (num > 35)
		return ('\0');

	if (num < 10)
		return ('0' + num);

	return ('A' + (num - 10));
}

#endif /* BIGINT_INT_TO_CHAR */
