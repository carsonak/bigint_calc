#ifndef PARSE_NUMBER_H
#define PARSE_NUMBER_H

#include "attribute_macros.h"

#include <stdio.h>  // FILE, *printf

#include "_numstr_alloc.h"
#include "bigint_types.h"

/* Minimum supported radix. */
#define RADIX_MINIMUM 2U
/* Maximum supported radix. */
#define RADIX_MAXIMUM 36U

len_ty
_numstr_print(FILE *const restrict stream, const numstr *const restrict num);

/*!
 * @brief a pointer to a function that maps one character to another.
 * @param c the character to be transformed.
 * @param args pointer to more arguments that can be passed to the function.
 *
 * @return the transformed character.
 */
typedef char (*mapping_func)(const char c, void *args);

char *filter_str(
	char const *const restrict str, len_ty *const restrict processed,
	const mapping_func map, void *const map_args
);
numstr *_numstr_new(
	char const *const restrict num_str, const radix_ty base,
	len_ty *const restrict processed
);
bigint *_numstr_to_bi(const numstr *const restrict nstr);
bigint *_anybase_to_bi(const numstr *const restrict num);

#endif  // PARSE_NUMBER_H
