#include "infiX.h"

/**
 * str_to_intarray - convert a string of numbers to a uint32_t u32array.
 * @num_str: a pointer to a string of numbers
 *
 * Description: This function converts a string of numbers to a uint32_t u32array.
 * The u32array will be in little endian order whereby the lower value numbers
 * will be placed in the lower indices. Index 0 will have a value indicating
 * the size of the u32array.
 *
 * Return: pointer to an uint32_t u32array, NULL on failure
 */
uint32_t *str_to_intarray(const char *num_str)
{
	uint32_t *u32array = NULL;
	size_t arr_size = 0, h = 0;
	int i = 0, g = 0;
	str_array *attrs = NULL;

	attrs = parse_numstr(num_str);
	if (!num_str || !num_str[0] || (attrs && !attrs->digits))
	{
		u32array = check_calloc(2, sizeof(*u32array));
		if (u32array)
			u32array[0] = 1;

		if (attrs)
			free_n_null(attrs->str);

		free_n_null(attrs);
		return (u32array);
	}

	if (!attrs)
		return (NULL);

	arr_size = (attrs->digits / MAX_DIGITS_u4b) + ((attrs->digits % MAX_DIGITS_u4b) ? 1 : 0);
	u32array = check_calloc((arr_size + 1), sizeof(*u32array));
	if (!u32array)
	{
		free_n_null(attrs);
		return (NULL);
	}

	/*Index 0 will have the size of the u32array*/
	u32array[0] = arr_size;
	/*The number in the string will be read from the least significant digit*/
	for (h = 1, g = (attrs->len - 1); h <= arr_size && g >= 0; h++)
	{
		i = 0;
		while (i < MAX_DIGITS_u4b && (g - i) >= 0)
		{
			if (attrs->str[g - i] >= '0' && attrs->str[g - i] <= '9')
			{
				u32array[h] += (attrs->str[g - i] - '0') * (uint32_t)(pow(10, i));
				i++;
			}
			else
				g--;
		}

		g -= i;
	}

	if (attrs->is_negative && u32array[arr_size] > 0)
		u32array[arr_size] |= NEGBIT_u4b;

	free_n_null(attrs->str);
	free_n_null(attrs);
	return (u32array);
}

/**
 * parse_numstr - strip the string of some leading chars and collect info
 * @num_str: a string of numbers.
 *
 * Description: the string will be stripped of leading "0", "," or " ". The
 * returned struct will store a copy of this resultant string, its length and
 * number of digits in it.
 *
 * Return: a struct with useful info about the number string.
 */
str_array *parse_numstr(const char *num_str)
{
	size_t i = 0;
	str_array ns = {(uint8_t *)num_str, 1, 0, 0}, *attributes = NULL;

	if (!num_str || !num_str[0])
		return (NULL);

	if (ns.str[0] == '-')
	{
		ns.str++;
		ns.is_negative = 1;
	}

	ns.str += padding_chars_len((char *)ns.str, "0, ");
	for (i = 0; ns.str && ns.str[i]; i++)
	{
		if (ns.str[i] >= '0' && ns.str[i] <= '9')
		{
			ns.digits++;
			ns.len = i + 1;
		}
		else if (ns.str[i] != ',' && ns.str[i] != ' ')
		{
			if ((ns.str[i] >= 'a' && ns.str[i] <= 'z') ||
				(ns.str[i] >= 'A' && ns.str[i] <= 'Z'))
				panic("base");
			else
				panic("invalid char");

			return (NULL);
		}
	}

	attributes = check_calloc(1, sizeof(*attributes));
	if (attributes)
	{
		attributes->str = (uint8_t *)strndup((char *)ns.str, ns.len);
		if (!attributes->str)
		{
			perror("Malloc fail");
			free_n_null(attributes);
			return (NULL);
		}

		attributes->len = ns.len;
		attributes->digits = ns.digits;
		attributes->is_negative = ns.is_negative;
	}

	return (attributes);
}

/**
 * intarr_to_str - convert a uint32_t u32array to a string of numbers.
 * @u32array: a uint32_t u32array
 *
 * Description: This function converts a uint32_t u32array to a string of numbers.
 * The u32array should be in little endian order whereby the lower value numbers
 * will be placed in the lower indices. Index 0 will have a value indicating
 * the size of the u32array.
 *
 * Return: a pointer to a string of numbers, NULL on failure
 */
char *intarr_to_str(uint32_t *u32array)
{
	size_t arr_size = 0, len = 0, g = 0, h = 0, i = 0;
	char *num_str = NULL, negative = '\0';
	ssize_t temp = 0, div = 1;

	if (!u32array)
		return (NULL);

	arr_size = u32array[0];
	/*Checking if the number is negative*/
	if (u32array[arr_size] > NEGBIT_u4b)
	{
		u32array[arr_size] ^= NEGBIT_u4b;
		len += 1;
		negative = '-';
	}

	trim_u4b_array(u32array);
	arr_size = u32array[0];
	len += arr_size * MAX_DIGITS_u4b;
	num_str = check_calloc((len + 1), sizeof(*num_str));
	if (!num_str)
		return (NULL);

	if (negative)
		num_str++;

	temp = u32array[arr_size];
	while (temp / div >= 10)
		div *= 10;

	for (h = arr_size, g = 0; h > 0 && g < len; h--)
	{
		temp = u32array[h];
		for (i = 0; div && (g + i) < len; i++)
		{
			num_str[g + i] = (temp / div) + '0';
			temp %= div;
			div /= 10;
		}

		g += i;
		div = (MAX_VAL_u4b / 10);
	}

	if (negative)
	{
		num_str--;
		num_str[0] = '-';
	}

	return (num_str);
}

/**
 * trim_u4b_array - trims zeros from end of an array
 * @arr: pointer to a u4b_array struct
 */
void trim_u4b_array(u4b_array *arr)
{
	if (!arr)
		return;

	if (!arr->array)
	{
		arr->len = 0;
		arr->is_negative = 0;
		return;
	}

	while (!arr->array[arr->len - 1] && arr->len > 1)
		--arr->len;
}
