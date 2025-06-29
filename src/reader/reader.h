#ifndef READER_H
#define READER_H

#include <stdbool.h>  // bool
#include <stdio.h>    // FILE

#include "types.h"

struct reader
{
	const char *restrict prompt;
	FILE *restrict stream;
	len_ty line, column;
	bool is_interactive, got_error;
};

typedef struct reader reader;

char reader_peekc(reader *const restrict self);
char reader_getc(reader *const restrict self);

#endif  // READER_H
