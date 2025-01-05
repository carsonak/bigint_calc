#include "_bigint_struct.h"
#include "bigint.h"

/**
 * struct numstr - a string representing a number.
 * @len: length of the string.
 * @is_negative: a bool indicating signedness of the number.
 * @str: the string.
 */
typedef struct numstr
{
	size_t len;
	bool is_negative;
	char *str;
} numstr;

static ATTR_NONNULL void *free_numstr(numstr *freeable_ptr);
static ATTR_MALLOC ATTR_MALLOC_FREE(free_numstr)
numstr *alloc_numstr(size_t len);

static ATTR_NONNULL bool
check_is_negative(char const *const num_str, size_t *str_i);
static bigint *anybase_to_bni(numstr *num, unsigned short int base);
static int char_to_int(const char c);

/**
 * alloc_numstr - allocate memory for a numstr of given length.
 * @len: length of the array, length 0 returns the struct with a NULL array.
 *
 * Return: a pointer to a numstr struct, NULL on failure.
 */
static numstr *alloc_numstr(size_t len)
{
	numstr *arr = xcalloc(1, sizeof(*arr));

	if (!arr)
		return (NULL);

	arr->len = len;
	if (len > 0)
	{
		arr->str = xmalloc(len * sizeof(*arr->str));
		if (!arr->str)
			arr = free_n_null(arr);
	}

	return (arr);
}

/**
 * free_numstr - free a numstr, return NULL.
 * @freeable_ptr: a pointer to a numstr.
 *
 * Return: NULL always.
 */
static void *free_numstr(numstr *freeable_ptr)
{
	freeable_ptr->len = 0;
	freeable_ptr->str = free_n_null(freeable_ptr->str);

	return (free_n_null(freeable_ptr));
}

/**
 * char_to_int - map a base36 ascii symbol to a decimal.
 * @c: an alphanumeric symbol.
 *
 * Return: decimal value of the symbol, -1 if invalid symbol.
 */
static int char_to_int(const char c)
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
char int_to_char(const unsigned int num)
{
	if (num > 35)
		return ('\0');

	if (num < 10)
		return ('0' + num);

	return ('A' + (num - 10));
}

/**
 * map_digits - validates that characters are acceptable digits.
 * @c: the character to validate.
 * @radix: pointer to an int indicating the current working radix.
 *
 * Return: the transformed character, -1 on error.
 */
static char map_digits(const char c, void const *const radix)
{
	int a = char_to_int(c);

	if (!radix)
		return (-1);

	const unsigned int base = *((const unsigned int *)radix);

	if (c == '_')
		return (0); /*underscores should be ignored.*/

	if (a < 0 || (unsigned int)a >= base)
		return (-1); /*invalid character.*/

	return (int_to_char(a));
}

/**
 * check_is_negative - determine signedness of number.
 * @num_str: a string with a number.
 * @str_i: pointer to num_str's index.
 *
 * Return: true if the sign is negative, false otherwise.
 */
static bool check_is_negative(char const *const num_str, size_t *str_i)
{
	bool is_neg = false;

	for (; num_str[*str_i] == '-' || num_str[*str_i] == '+'; ++(*str_i))
		if (num_str[*str_i] == '-')
			is_neg = !is_neg;

	return (is_neg);
}

/**
 * str_to_numstr - parse a string of digits.
 * @num_str: a string with a number.
 * @base: an int between 2-36 indicating the base of the number.
 * @processed: address to store number of characters processed.
 *
 * If this function returns NULL, processed will not be changed.
 *
 * Return: pointer to a numstr struct, NULL on failure.
 */
static numstr *str_to_numstr(
	char const *const num_str, const unsigned int base, size_t *const processed)
{
	numstr *ns = NULL;
	size_t str_i = 0, p = 0;

	if (!num_str || base < 2 || base > 36)
		return (NULL);

	ns = alloc_numstr(0);
	if (!ns)
		return (NULL);

	ns->is_negative = check_is_negative(num_str, &str_i);
	if (num_str[str_i] == '_')
	{
		fprintf(stderr, "ParsingError: Leading underscores not allowed.\n");
		goto cleanup_numstr;
	}

	str_i += leading_chars_span(&num_str[str_i], "0");
	ns->str = filter_str(&num_str[str_i], &p, map_digits, &base);
	if (!ns->str)
		goto critical_failure;

	ns->len = strlen(ns->str);
	if (!ns->len)
	{
		fprintf(
			stderr, "ParsingError: string did not contain any valid digits.\n");
		goto cleanup_numstr;
	}

	str_i += p;
	if (str_i && num_str[str_i - 1] == '_')
	{
		fprintf(stderr, "ParsingError: Trailing underscores not allowed.\n");
		--str_i;
cleanup_numstr:
		ns = free_numstr(ns);
	}

	if (processed)
		*processed = str_i;

	return (ns);

critical_failure:
	return (free_numstr(ns));
}

/**
 * numstr_to_bi - convert a numstr to a bigint.
 * @nstr: the numstr.
 *
 * Return: a pointer to a bigint_i struct, NULL on failure.
 */
bigint *numstr_to_bi(numstr *nstr)
{
	size_t bi_i = 0, nstr_i = 0, tmp = 0;
	unsigned int digits = 0;
	bigint *bi_arr = NULL;
	char num_buf[16], *end = NULL;

	if (!nstr || !nstr->len || !nstr->str || !isalnum(nstr->str[0]))
		return (NULL);

	/*sizeof(bigint_i) == */
	/*ceil(numstr.len / no. of digits that can represent BIGINT_BASE)*/
	digits = count_digits(BIGINT_BASE - 1);
	bi_i = (nstr->len / digits);
	if (nstr->len % digits)
		++bi_i;

	bi_arr = _bi_alloc(bi_i);
	if (!bi_arr)
		return (NULL);

	for (bi_i = 0, nstr_i = nstr->len; bi_i < bi_arr->len && nstr_i; ++bi_i)
	{
		if (nstr_i >= digits)
			nstr_i -= digits;
		else
		{
			digits = nstr_i;
			nstr_i = 0;
		}

		strncpy(num_buf, &nstr->str[nstr_i], digits);
		num_buf[digits] = '\0';
		tmp += strtoull(num_buf, &end, 10);
		if (*end)
		{
			fprintf(stderr, "ParsingError: Invalid character '%c'\n", *end);
			return (bi_delete(bi_arr));
		}

		bi_arr->num[bi_i] = tmp % BIGINT_BASE;
		tmp /= BIGINT_BASE;
	}

	while (bi_i < bi_arr->len && tmp)
	{
		bi_arr->num[bi_i] = tmp % BIGINT_BASE;
		tmp /= BIGINT_BASE;
		++bi_i;
	}

	_bi_trim(bi_arr);
	return (bi_arr);
}

/**
 * anybase_to_bni - convert a numstr in any base to a bigint.
 * @num: the numstr.
 * @base: an int between 2-36 indicating the base of the number.
 *
 * Return: a pointer to a bigint struct, NULL on failure.
 */
static bigint *anybase_to_bni(numstr *num, unsigned short int base)
{
	size_t i = 0;
	int cval = 0;
	bigint *bigint_final = NULL, *tmp = NULL;
	bigint base_bn = {.len = 1, .is_negative = false, .num = (u_int[1]){base}};
	bigint char_bn = {.len = 1, .is_negative = false, .num = (u_int[1]){0}};

	if (!num || !num->str || base < 2 || base > 36)
		return (NULL);

	bigint_final = _bi_alloc(1);
	if (!bigint_final)
		return (NULL);

	/* while str[i]; num = base * num + str[i]; ++i; */
	for (i = 0; i < num->len && bigint_final; ++i)
	{
		tmp = bigint_final;
		bigint_final = bi_multiply(&base_bn, bigint_final);
		tmp = bi_delete(tmp);
		cval = char_to_int(num->str[i]);
		if (cval < 0 || (u_int)cval >= base)
		{
			fprintf(
				stderr, "ParsingError: Invalid character '%c' for base%u\n",
				num->str[i], base);
			return (bi_delete(bigint_final));
		}

		char_bn.num[0] = cval;
		tmp = bigint_final;
		bigint_final = bi_add(bigint_final, &char_bn);
		tmp = bi_delete(tmp);
	}

	return (bigint_final);
}

/*!
 * @brief returns a pointer to a new bigint type.
 * @param num_str number to initialise bigint with.
 * @param base radix used to intepret the number in `num_str`.
 * @param processed variable used to store number of characters processed.
 * @return pointer to the bigint, NULL on failure.
 */
bigint *bi_new(
	char const *const num_str, unsigned short int base, size_t *const processed)
{
}
