#include "tests.h"

static bignum_i *n;

/**
 * setup - setup some variables.
 */
void setup(void)
{
	n = NULL;
}

/**
 * teardown - cleanup after tests.
 */
void teardown(void)
{
	bni_free(n);
}

TestSuite(zero_len, .init = setup, .fini = teardown);

Test(zero_len, test_alloc_zero_len_bignum,
	 .description = "allocate size 0", .timeout = 1.0)
{
	n = bni_alloc(0);

	cr_assert(zero(sz, n->len));
	cr_assert(zero(ptr, n->num));
}

Test(zero_len, test_resize_NULL_to_zero_len_bignum,
	 .description = "resize null pointer to 0", .timeout = 1.0)
{
	cr_assert(eq(chr, bn_realloc(n, 0), false));
}

Test(zero_len, test_resize_zero_len_to_zero_len_bignum,
	 .description = "resize a 0 len bignum to 0", .timeout = 1.0)
{
	n = bni_alloc(0);

	cr_assert(eq(chr, bn_realloc(n, 0), true));
	cr_assert(zero(sz, n->len));
	/*cr_assert(zero(ptr, n->num)); //implementation defined.*/
}

Test(zero_len, test_resize_to_zero_len_bignum,
	 .description = "resize a bignum to 0", .timeout = 1.0)
{
	n = bni_alloc(5);

	cr_assert(eq(chr, bn_realloc(n, 0), true));
	cr_assert(zero(sz, n->len));
	/*cr_assert(zero(ptr, n->num)); //implementation defined.*/
}
