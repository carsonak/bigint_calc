#include <readline/history.h>   // history
#include <readline/readline.h>  // readline

#include "reader.h"
#include "xalloc.h"

static void _reader_update_stream(
	const char *const restrict prompt, FILE *const restrict stream
) _nonnull_pos(2);

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

	if (!curr_line)
		return;

	if (*curr_line == '\0')
	{
		fseek(stream, -1, SEEK_CUR);
		goto cleanup;
	}

	long int stream_offset = 0;
	int i = 1;
	HIST_ENTRY *const *restrict hist = history_list();

	for (; i <= 3 && history_length - i >= 0; i++)
	{
		HIST_ENTRY *restrict h = hist[history_length - i];

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
	if (history_length - i < 0)
		i--;

	hist = history_list();
	for (int h_i = history_length - i; h_i < history_length; h_i++)
	{
		HIST_ENTRY *restrict h = hist[h_i];

		if (h)
			fprintf(stream, "%s\n", h->line);
	}

	stream_offset = strlen(curr_line) + 1;
	fseek(stream, -stream_offset, SEEK_CUR);
cleanup:
	xfree((void *)curr_line);
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

	if (self->is_interactive && c == EOF)
	{
		_reader_update_stream(self->prompt, self->stream);
		c = fgetc(self->stream);
	}

	if (c == EOF)
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

	const len_ty line = self->line, column = self->column;
	const char c = reader_getc(self);

	self->line = line;
	self->column = column;
	if (c != EOF)
		ungetc(c, self->stream);

	return (c);
}

string *reader_getline(reader *const restrict self)
{
	if (!self)
		return (NULL);

	char buffer[512], c;
	string_view l = {0};
	string *restrict line = NULL;
	unsigned int buf_i = 0;

	do
	{
		c = reader_getc(self);
		if (c == EOF)
			break;

		if (buf_i >= sizeof(buffer) - 1)
		{
			buffer[buf_i] = 0;
			string *const old = line;

			line = string_cat(
				*string_to_string_view(&l, old),
				(string_view){.len = buf_i, .s = buffer}
			);
			string_delete(old);
			buf_i = 0;
			if (!line)
				goto error;
		}

		buffer[buf_i++] = c;
	} while (c != '\n');

	buffer[buf_i] = 0;
	string *const old = line;

	line = string_cat(
		*string_to_string_view(&l, old),
		(string_view){.len = buf_i, .s = buffer}
	);
	string_delete(old);
	buf_i = 0;
	if (!line)
	{
error:
		self->got_error = true;
	}

	return (line);
}
