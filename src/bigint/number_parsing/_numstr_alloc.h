#ifndef NUMSTR_INTERNALS_H
#define NUMSTR_INTERNALS_H

#include <stdbool.h> /* bool */

#include "attribute_macros.h"
#include "types.h"
#include "xalloc.h"

/**
 * struct numstr - a string representing a number.
 * @len: length of the string.
 * @is_negative: a bool indicating signedness of the number.
 * @str: the string.
 */
struct numstr
{
	len_type len;
	bool is_negative;
	char *restrict str;
};

typedef struct numstr numstr;

void *_numstr_free(numstr *freeable_ptr);
numstr *_numstr_alloc(len_type len) ATTR_MALLOC ATTR_MALLOC_FREE(_numstr_free);

#endif /* NUMSTR_INTERNALS_H */
