#include "tests.h"

TestSuite(memory_allocation_failure);

Test(memory_allocation_failure, test_bi_alloc,
	 .description = "bi_alloc() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_alloc(10)));
}

Test(memory_allocation_failure, test_bi_realloc,
	 .description = "bi_realloc() returns false", .timeout = 1.0)
{
	bigint *n = NULL;

	cr_assert(zero(chr, bi_realloc(n, 10)));
}

Test(memory_allocation_failure, test_bi_dup,
	 .description = "bi_dup() returns NULL", .timeout = 1.0)
{
	bigint n = {.len = 1, .is_negative = false, .num = (uint[]){10}};

	cr_assert(zero(ptr, bi_dup(&n)));
}

Test(memory_allocation_failure, test_bi_swap,
	 .description = "bi_swap() returns false", .timeout = 1.0)
{
	bigint n1 = {.len = 1, .is_negative = false, .num = (uint[]){10}};
	bigint n2 = {.len = 2, .is_negative = false, .num = (uint[]){10, 10}};

	cr_assert(zero(chr, bi_swap(&n1, &n2)));
}
