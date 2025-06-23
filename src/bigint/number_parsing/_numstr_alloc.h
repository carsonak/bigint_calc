#ifndef NUMSTR_INTERNALS_H
#define NUMSTR_INTERNALS_H

#include <stdbool.h>  // bool

#include "attribute_macros.h"
#include "types.h"
#include "xalloc.h"

/*!
 * @brief a string representing a number.
 */
struct numstr
{
	/*! @protected length of the string. */
	len_ty len;
	/*! @protected a bool indicating signedness of the number. */
	bool is_negative;
	/*! @protected an int in the range 2-36 indicating the radix of the number. */
	radix_ty base;
	/*! @protected pointer to the string. */
	char *restrict str;
};

typedef struct numstr numstr;

void *_numstr_free(numstr *freeable_ptr);
numstr *_numstr_alloc(len_ty len) _malloc _malloc_free(_numstr_free);

#endif  // NUMSTR_INTERNALS_H
