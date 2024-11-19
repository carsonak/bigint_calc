#include "parsing.h"

/**
 * filter_str - translates all characters of a string according to a function.
 * @str: the string to process.
 * @processed: pointer to store number of characters processed in the string.
 * @map: pointer to a function that transforms maps characters onto others.
 * @map_args: pointer to more arguments to pass to map.
 *
 * If map returns a negative char, processing of the string will be stoped and
 * the processed string will be returned upto the character.
 * If map returns 0, the character will be ignored, essentially deleting the
 * character from the output.
 *
 * Return: pointer to the filtered string, NULL on failure.
 */
char *filter_str(const char *str, size_t *const processed,
				 const mapping_func map, void *map_args)
{
	const unsigned int buf_size = 1024;
	char *buffer = NULL, *output = NULL, c = 0;
	size_t buf_i = 0, o_len = 0;

	if (!str || !processed || !map)
		return (NULL);

	buffer = xmalloc(buf_size);
	if (!buffer)
		return (NULL);

	*processed = 0;
	while (str[*processed])
	{
		for (buf_i = 0; buf_i < buf_size - 1; (*processed)++)
		{
			c = map(str[*processed], map_args);
			if (c < 0)
				break;

			if (c == 0)
				continue;

			buffer[buf_i] = c;
			buf_i++;
		}

		buffer[buf_i] = '\0';
		output = xrealloc(output, (o_len + buf_i + 1) * sizeof(*output));
		if (!output)
			break;

		strcpy(&output[o_len], buffer);
		o_len += buf_i;
		if (c < 0)
			break;
	}

	(*processed)++;
	return (output);
}
