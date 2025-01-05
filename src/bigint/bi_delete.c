#include "_bigint_struct.h"

/**
 * bi_delete - free a bigint, return NULL.
 * @freeable_ptr: a pointer to a bigint.
 *
 * Return: NULL always.
 */
void *bi_delete(bigint *const freeable_ptr)
{
    if (freeable_ptr)
    {
        freeable_ptr->len = 0;
        // freeable_ptr->num = free_n_null(freeable_ptr->num);
        freeable_ptr->num = NULL;
    }

    return (free_n_null(freeable_ptr));
}
