#ifndef BIGINT_XALLOC_H
#define BIGINT_XALLOC_H

#include "macros.h"

#include <stdio.h>  /* perror */
#include <stdlib.h> /* *alloc */

#include "types.h"

void *xfree(void *const freeable_ptr);
/* clang-format off */
void *xcalloc(const len_type items, const len_type sizeof_item) ATTR_MALLOC ATTR_MALLOC_FREE(xfree) ATTR_ALLOC_SIZE(1, 2);
void *xmalloc(const len_type size) ATTR_MALLOC ATTR_MALLOC_FREE(xfree) ATTR_ALLOC_SIZE(1);
void *xrealloc(void *nullable_ptr, const len_type size) ATTR_ALLOC_SIZE(2);
void *xrealloc_free_on_fail(void *nullable_ptr, const len_type size) ATTR_ALLOC_SIZE(2);
/* clang-format on */
char *xstrdup(char const *const str) ATTR_MALLOC ATTR_MALLOC_FREE(xfree);

#endif /* BIGINT_XALLOC_H */
