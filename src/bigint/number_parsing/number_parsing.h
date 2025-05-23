#ifndef PARSE_NUMBER_H
#define PARSE_NUMBER_H

#include "attribute_macros.h"

#include <ctype.h>    /* isalnum, isdigit, toupper */
#include <inttypes.h> /* strtoumax */
#include <string.h>   /* strlen */

#include "_numstr_alloc.h"
#include "bigint_types.h"

/* Minimum supported radix. */
#define RADIX_MINIMUM 2U
/* Maximum supported radix. */
#define RADIX_MAXIMUM 36U

len_type
_numstr_print(FILE *const restrict stream, const numstr *const restrict num);

/**
 * mapping_func - a pointer to a function that maps one character to another.
 * @c: the character to be transformed.
 * @args: pointer to more arguments that can be passed to the function.
 *
 * Return: the transformed character.
 */
typedef char (*mapping_func)(const char c, void *args);

char *filter_str(
	char const *const restrict str, len_type *const restrict processed,
	const mapping_func map, void *const map_args
);
numstr *_str_to_numstr(
	char const *const restrict num_str, const radix_type base,
	len_type *const restrict processed
);
bigint *_numstr_to_bi(numstr *nstr);
bigint *_anybase_to_bi(numstr *num, const radix_type base);
char *bi_tostr(bigint const *const n);

#endif /* PARSE_NUMBER_H */
