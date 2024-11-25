#include "tests.h"

TestSuite(memory_allocation_failure);

Test(memory_allocation_failure, test_bn_alloc,
	 .description = "bn_alloc() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_alloc(10)));
}

Test(memory_allocation_failure, test_bn_realloc,
	 .description = "bn_realloc() returns false", .timeout = 1.0)
{
	bignum_i *n = NULL;

	cr_assert(zero(chr, bn_realloc(n, 10)));
}

Test(memory_allocation_failure, test_bn_dup,
	 .description = "bn_dup() returns NULL", .timeout = 1.0)
{
	bignum_i n = {.len = 1, .is_negative = false, .num = (uint[]){10}};

	cr_assert(zero(ptr, bn_dup(&n)));
}

Test(memory_allocation_failure, test_bn_swap,
	 .description = "bni_swap() returns false", .timeout = 1.0)
{
	bignum_i n1 = {.len = 1, .is_negative = false, .num = (uint[]){10}};
	bignum_i n2 = {.len = 2, .is_negative = false, .num = (uint[]){10, 10}};

	cr_assert(zero(chr, bni_swap(&n1, &n2)));
}
