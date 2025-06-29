#include "reader.h"
#include "xalloc.h"

#include <readline/history.h>   // history
#include <readline/readline.h>  // readline

/*!
 * @brief prompt the user for more input and update history.
 * @private @memberof reader
 *
 * @param prompt prompt to use.
 * @param stream pointer to the history file stream.
 */
static void _reader_update_stream(
	const char *const restrict prompt, FILE *const restrict stream
)
{
	const char *const restrict curr_line = readline(prompt);

	if (!stream || !curr_line)
		return;

	if (*curr_line == '\0')
	{
		fseek(stream, -1, SEEK_CUR);
		goto cleanup;
	}

	long int stream_offset = 0;
	int i = 1;

	for (; i <= 3; i++)
	{
		const HIST_ENTRY *restrict h = history_get(history_length - i);

		if (!h)
			break;

		stream_offset += strlen(h->line) + 1;
		if (strcmp(h->line, curr_line) == 0)
		{
			// does this leave a hole in the array?
			free_history_entry(remove_history(history_length - i));
			break;
		}
	}

	add_history(curr_line);
	fseek(stream, -stream_offset, SEEK_CUR);
	if (history_length - i < history_base)
		i--;

	for (int h_i = history_length - i; h_i < history_length; h_i++)
	{
		const HIST_ENTRY *restrict h = history_get(h_i);

		fprintf(stream, "%s\n", h->line);
	}

	stream_offset = strlen(curr_line) + 1;
	fseek(stream, -stream_offset, SEEK_CUR);
cleanup:
	xfree(curr_line);
}

/*!
 * @brief read one character from the stream.
 * @public @memberof reader
 *
 * @param self pointer to a `reader` to use.
 * @return the character read, EOF on error.
 */
char reader_getc(reader *const restrict self)
{
	if (!self)
		return (EOF);

	char c = fgetc(self->stream);

	if (self->is_interactive && feof(self->stream))
	{
		_reader_update_stream(self->prompt, self->stream);
		c = fgetc(self->stream);
	}

	if (feof(self->stream))
		return (EOF);

	self->column++;
	if (c == '\n')
	{
		self->line++;
		self->column = 0;
	}

	return (c);
}

/*!
 * @brief get the next character in the stream without advancing the stream.
 * @public @memberof reader
 *
 * @param self pointer to a `reader` to use.
 * @return the next character in the stream, EOF on error.
 */
char reader_peekc(reader *const restrict self)
{
	if (!self)
		return (EOF);

	const char c = reader_getc(self);
	if (!feof(self->stream))
		ungetc(c, self->stream);

	return (c);
}
