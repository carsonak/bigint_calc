#include "tests.h"

bignum_i num1 = {.len = 1, .is_negative = false, .num = (uint[]){1}};
bignum_i num2 = {.len = 1, .is_negative = false, .num = (uint[]){1}};

TestSuite(memory_allocation_failure);

Test(memory_allocation_failure, test_bn_add,
	 .description = "bni_add() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_add(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_add_int,
	 .description = "bni_add_int() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_add_int(&num1, 1)));
}

Test(memory_allocation_failure, test_bn_divide,
	 .description = "bni_divide() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_divide(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_multiply,
	 .description = "bni_multiply() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_multiply(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_power,
	 .description = "bni_power() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_power(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_subtract,
	 .description = "bni_subtract() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_subtract(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_subtract_int,
	 .description = "bni_subtract_int() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bni_subtract_int(&num1, 1)));
}
