#ifndef NUMSTR_H
#define NUMSTR_H

#include "macros.h"

#include <stdio.h>   /* *printf */
#include <stdbool.h> /* bool */
#include <stddef.h>  /* size_t, ptr_diff */
#include <stdlib.h>  /*strtoull*/
#include <string.h>  /* strlen, strcpy */

#include "xalloc.h"
#include "bignum_utils.h"
#include "bignum_math.h"
#include "string_utils.h"

/**
 * struct numstr - a string representing a number.
 * @len: length of the string.
 * @is_negative: a bool indicating signedness of the number.
 * @str: the string.
 */
typedef struct numstr
{
    size_t len;
    bool is_negative;
    char *str;
} numstr;

/* alloc_funcs */

void *free_numstr(numstr *freeable_ptr);
ATTR_MALLOC
ATTR_MALLOC_FREE(free_numstr)
numstr *alloc_numstr(size_t len);

unsigned int count_digits(size_t num);
numstr *str_to_numstr(
    char const *const str, const unsigned int base, size_t *const processed);
bignum_i *numstr_to_bni(numstr *num);
numstr *bni_to_numstr(bignum_i *bn_arr, const unsigned int base);
bignum_i *anybase_to_bni(numstr *nstr, const unsigned int base);

#endif /*NUMSTR_H*/
