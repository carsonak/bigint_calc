#include "tests.h"

size_t processed;

/**
 * capitalise - transform letters to upper case.
 * @c: the letter.
 * @args: is ignored.
 *
 * Return: the capitalised letter, 0 if digit, -1 otherwise.
 */
static char capitalise(char c, void *args)
{
	(void)args;
	if (isalpha(c))
		return (toupper(c));

	if (isdigit(c))
		return (0);

	return (-1);
}

/**
 * rand_str - generate a string of random characters of a given size.
 * @dest: pointer to a memory location to store the string.
 * @len: number of characters to generate.
 *
 * Return: pointer to dest.
 */
static char *rand_str(char *dest, size_t len)
{
	char charset[] = " \t\n"
					 "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
					 "0123456789"
					 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					 "abcdefghijklmnopqrstuvwxyz";

	while (len--)
		*dest++ = charset[(size_t)(rand() % (sizeof(charset) - 1))];

	*dest = '\0';
	return (dest);
}

/**
 * setup - setup some variables.
 */
void setup(void)
{
	processed = 0;
}

/**
 * teardown - cleanup after tests.
 */
void teardown(void) {}

TestSuite(NULL_inputs, .init = setup, .fini = teardown);

Test(NULL_inputs, test_all_NULL_inputs,
	 .description = "filter_str(NULL, NULL, NULL, NULL) => NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, filter_str(NULL, NULL, NULL, NULL)));
}

Test(NULL_inputs, test_str_is_NULL,
	 .description = "filter_str(NULL, &processed, capitalise, NULL) => NULL",
	 .timeout = 1.0)
{
	cr_assert(zero(ptr, filter_str(NULL, &processed, capitalise, NULL)));
}

Test(NULL_inputs, test_processed_is_NULL,
	 .description = "filter_str(str, NULL, capitalise, NULL) => NULL",
	 .timeout = 1.0)
{
	const char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	cr_assert(zero(ptr, filter_str(str, NULL, capitalise, NULL)));
}

Test(NULL_inputs, test_capitalise_is_NULL,
	 .description = "filter_str(str, &processed, NULL, NULL) => NULL",
	 .timeout = 1.0)
{
	const char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	cr_assert(zero(ptr, filter_str(str, &processed, NULL, NULL)));
}
