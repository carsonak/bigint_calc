#ifndef BIGINT_CHAR_INT_CONVERSION
#define BIGINT_CHAR_INT_CONVERSION

static int numchar_to_int(const char c);
static char int_to_char(const unsigned int num);

/**
 * numchar_to_int - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbol.
 */
static int numchar_to_int(const char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

/**
 * int_to_char - map a decimal between 0-35 to a base36 ascii symbol.
 * @num: the number to convert.
 *
 * Return: the ascii symbol, '\0' on error.
 */
static char int_to_char(const unsigned int num)
{
	if (num > 35)
		return ('\0');

	if (num < 10)
		return ('0' + num);

	return ('A' + (num - 10));
}

#endif /* BIGINT_CHAR_INT_CONVERSION */
