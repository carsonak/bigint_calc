#ifndef NUMSTR_INTERNALS_H
#define NUMSTR_INTERNALS_H

#include "macros.h"

#include <stdbool.h> /* bool */

#include "types.h"
#include "xalloc.h"

/**
 * struct numstr - a string representing a number.
 * @len: length of the string.
 * @str: the string.
 * @is_negative: a bool indicating signedness of the number.
 */
struct numstr
{
	len_type len;
	char *restrict str;
	bool is_negative;
};

typedef struct numstr numstr;

void *_numstr_free(numstr *freeable_ptr);
ATTR_MALLOC ATTR_MALLOC_FREE(_numstr_free)
numstr *_numstr_alloc(len_type len);

/* printing */

len_type
_numstr_print(FILE *const restrict stream, const numstr *const restrict num);

#endif /* NUMSTR_INTERNALS_H */
