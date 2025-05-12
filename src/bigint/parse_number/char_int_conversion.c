#ifndef BIGINT_CHAR_INT_CONVERSION
#define BIGINT_CHAR_INT_CONVERSION

static short int char_to_int(const char c);
static char int_to_char(const unsigned short int num);

/**
 * char_to_int - map an ascii symbol for base36 to a decimal number in the range 0-35.
 * @c: an alphanumeric character.
 *
 * Return: decimal value of the symbol, -1 on error.
 */
static short int char_to_int(const char c)
{
	if (!isalnum(c))
		return (-1);

	if (isdigit(c))
		return (c - '0');

	return (toupper(c) - 'A' + 10);
}

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

#endif /* BIGINT_CHAR_INT_CONVERSION */
