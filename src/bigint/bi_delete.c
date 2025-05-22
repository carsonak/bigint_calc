#include "_bi_internals.h"
#include "bigint.h"

/**
 * bi_delete - free a `bigint`, return NULL.
 * @ptr: a pointer to a `bigint`.
 *
 * Return: NULL always.
 */
void *bi_delete(bigint *const ptr) { return (_bi_free(ptr)); }
