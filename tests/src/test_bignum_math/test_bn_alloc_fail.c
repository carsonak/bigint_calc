#include "tests.h"

bignum num1 = {.len = 1, .is_negative = false, .num = (uint[]){1}};
bignum num2 = {.len = 1, .is_negative = false, .num = (uint[]){1}};

TestSuite(memory_allocation_failure);

Test(memory_allocation_failure, test_bn_add,
	 .description = "bn_add() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_add(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_add_int,
	 .description = "bn_add_int() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_add_int(&num1, 1)));
}

Test(memory_allocation_failure, test_bn_divide,
	 .description = "bn_divide() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_divide(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_multiply,
	 .description = "bn_multiply() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_multiply(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_power,
	 .description = "bn_power() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_power(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_subtract,
	 .description = "bn_subtract() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_subtract(&num1, &num2)));
}

Test(memory_allocation_failure, test_bn_subtract_int,
	 .description = "bn_subtract_int() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bn_subtract_int(&num1, 1)));
}
