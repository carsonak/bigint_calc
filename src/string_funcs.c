#include "infiX.h"

/**
 * padding_chars_len - finds the number of leading characters in a string
 * @str: the string to check
 * @ch: the character
 *
 * Return: number of leading characters
 */
size_t padding_chars_len(char *str, char *ch)
{
	size_t count = 0;

	if (str && ch && *ch)
	{
		count = strspn(str, ch);
		if (count && !str[count])
			count--;
	}

	return (count);
}

/**
 * print_u4b_array - print a u4b_array
 * @arr: pointer to the array struct
 */
void print_u4b_array(u4b_array *arr)
{
	size_t i = 0;

	if (!arr)
		return;

	printf("{ ");
	if (!arr->array)
		printf("NULL");

	for (i = 0; arr->array && i < arr->len - 1; i++)
		printf("%d, ", arr->array[i]);

	if (arr->array)
		printf("%d", arr->array[i]);

	if (arr->is_negative)
		printf(" } -\n");
	else
		printf(" }\n");
}
