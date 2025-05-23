#include "test_internals.h"

TEST(memory_allocation_failure, test_bi_alloc)
{
	CHECK(_bi_alloc(10) == NULL);
}

TEST(memory_allocation_failure, test_bi_realloc)
{
	bigint *n = NULL;

	CHECK(_bi_resize(n, 10) == NULL);
}

TEST(memory_allocation_failure, test_bi_dup)
{
	const bigint n = {
		.len = 1, .is_negative = false, .num = (u_int[]){DUMMY_VALUE}};

	CHECK(_bi_dup(&n) == NULL);
}
