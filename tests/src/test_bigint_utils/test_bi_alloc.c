#include "tests.h"

static bigint *n;

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
	bi_delete(n);
}

TestSuite(zero_len, .init = setup, .fini = teardown);

Test(zero_len, test_alloc_zero_len_bigint,
	 .description = "new(0), creates a 0 len bigint", .timeout = 1.0)
{
	n = bi_alloc(0);
	cr_assert(ne(ptr, n, NULL));

	cr_expect(zero(sz, n->len));
	cr_expect(zero(ptr, n->num));
}

Test(zero_len, test_resize_NULL_to_zero_len_bigint,
	 .description = "resize(NULL, 0) == new(0)", .timeout = 1.0)
{
	n = bi_resize(n, 0);

	cr_assert(ne(ptr, n, NULL));

	cr_expect(zero(sz, n->len));
	cr_expect(zero(ptr, n->num));
}

Test(zero_len, test_resize_zero_len_to_zero_len_bigint,
	 .description = "resize a 0 len bigint to 0", .timeout = 1.0)
{
	n = bi_alloc(0);
	bigint *const tmp = bi_resize(n, 0);

	cr_assert(ne(ptr, tmp, NULL));
	n = tmp;

	cr_expect(zero(sz, n->len));
	cr_expect(zero(ptr, n->num));
}

Test(zero_len, test_resize_to_zero_len_bigint,
	 .description = "resize a bigint to 0", .timeout = 1.0)
{
	n = bi_alloc(5);
	cr_assert(ne(ptr, n, NULL), "Failed to create bigint");
	bigint *const tmp = bi_resize(n, 0);

	cr_assert(ne(ptr, tmp, NULL), "Failed to resize bigint");
	n = tmp;

	cr_expect(zero(sz, n->len));
	cr_expect(zero(ptr, n->num));
}
