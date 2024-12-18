#include "tests.h"

bigint num1 = {.len = 1, .is_negative = false, .num = (uint[]){1}};
bigint num2 = {.len = 1, .is_negative = false, .num = (uint[]){1}};

TestSuite(memory_allocation_failure);

Test(memory_allocation_failure, test_bi_add,
	 .description = "bi_add() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_add(&num1, &num2)));
}

Test(memory_allocation_failure, test_bi_add_int,
	 .description = "bi_add_int() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_add_int(&num1, 1)));
}

Test(memory_allocation_failure, test_bi_divide,
	 .description = "bi_divide() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_divide(&num1, &num2)));
}

Test(memory_allocation_failure, test_bi_multiply,
	 .description = "bi_multiply() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_multiply(&num1, &num2)));
}

Test(memory_allocation_failure, test_bi_power,
	 .description = "bi_power() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_power(&num1, &num2)));
}

Test(memory_allocation_failure, test_bi_subtract,
	 .description = "bi_subtract() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_subtract(&num1, &num2)));
}

Test(memory_allocation_failure, test_bi_subtract_int,
	 .description = "bi_subtract_int() returns NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, bi_subtract_int(&num1, 1)));
}
