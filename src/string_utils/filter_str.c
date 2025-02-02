#include "parsing.h"

/**
 * filter_str - filter characters of a string.
 * @str: the string to process.
 * @processed: pointer to store number of characters processed in the string.
 * @map: pointer to a function that transforms/maps characters onto others.
 * @map_args: pointer to more arguments to pass to the map function.
 *
 * If map returns a negative char, processing is finalised and the processed
 * string returned.
 * If map returns 0, the character will be ignored, essentially deleting the
 * character from the returned string.
 *
 * If this function returns NULL, the value of processed will be unchanged.
 *
 * Return: pointer to the filtered string, NULL on failure.
 */
char *filter_str(char const *const str, size_t *const processed,
				 const mapping_func map, void const *const map_args)
{
	const unsigned int buf_size = 1024;
	char *output = NULL, c = 0;
	size_t buf_i = 0, str_i = 0, out_len = 0;

	if (!str || !map)
		return (NULL);

	char *const buffer = xmalloc(buf_size);

	if (!buffer)
		return (NULL);

	while (str[str_i])
	{
		for (buf_i = 0; buf_i < buf_size - 1 && str[str_i]; ++str_i)
		{
			c = map(str[str_i], map_args);
			if (c < 0)
				break;

			if (c == 0)
				continue;

			buffer[buf_i] = c;
			++buf_i;
		}

		buffer[buf_i] = '\0';
		output = xrealloc_free_on_fail(output, (out_len + buf_i + 1) * sizeof(*output));
		if (!output)
			break;

		strcpy(&output[out_len], buffer);
		out_len += buf_i;
		if (c < 0)
			break;
	}

	/*processed should not change in case of alloc fail.*/
	if (output && processed)
		*processed = str_i;

	xfree(buffer);
	/*In the case `str = "\0"`, output will be NULL, which is undesirable as*/
	/*NULL is an error value in this function.*/
	if (str_i == 0 && !str[str_i])
		return (xcalloc(1, sizeof(*output)));

	return (output);
}
