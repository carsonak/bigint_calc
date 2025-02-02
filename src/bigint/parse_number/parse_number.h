#ifndef PARSE_NUMBER_H
#define PARSE_NUMBER_H

#include "macros.h"

#include <stddef.h> /* size_t */
#include <string.h> /* strlen */

#include "bigint_typedefs.h"

/**
 * mapping_func - a pointer to a function that maps one character to another.
 * @c: the character to be transformed.
 * @args: pointer to more arguments that can be passed to the function.
 *
 * Return: the transformed character.
 */
typedef char (*mapping_func)(const char c, void *args);

char *filter_str(
	char const *const str, size_t *const processed, const mapping_func f,
	void *const f_args);
size_t leading_chars_span(char const *const str, char const *const ch);
numstr *str_to_numstr(
	char const *const num_str, const unsigned short int base,
	size_t *const processed);
bigint *numstr_to_bi(numstr *nstr);
bigint *anybase_to_bi(numstr *num, const unsigned short int base);

#endif /*PARSE_NUMBER_H*/
