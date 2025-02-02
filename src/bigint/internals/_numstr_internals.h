#ifndef NUMSTR_INTERNALS_H
#define NUMSTR_INTERNALS_H

#include "macros.h"

#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t */

#include "xalloc.h"

/**
 * struct numstr - a string representing a number.
 * @len: length of the string.
 * @str: the string.
 * @is_negative: a bool indicating signedness of the number.
 */
struct numstr
{
	size_t len;
	char *str;
	bool is_negative;
};

typedef struct numstr numstr;

ATTR_NONNULL void *_numstr_free(numstr *freeable_ptr);
ATTR_MALLOC ATTR_MALLOC_FREE(_numstr_free)
numstr *_numstr_alloc(size_t len);

#endif /* NUMSTR_INTERNALS_H */
