#include "tests.h"

/**
 * setup_suite1 - setup
 */
void setup_suite1(void)
{
}

/**
 * teardown_suite1 - teardown
 */
void teardown_suite1(void)
{
}

TestSuite(suite1, .init = setup_suite1, .fini = teardown_suite1);

Test(suite1, test_0)
{
	mid_uint num[2] = {1, 0};
	mid_uint *output = infiX_addition(num, num);

	cr_expect(eq(u32[2], num, output), "{1, 0} + {1, 0} == {1, 0}");
	/*cr_expect_arr_eq(&num, output, sizeof(num), "{1, 0} + {1, 0} == {1, 0}");*/
}
