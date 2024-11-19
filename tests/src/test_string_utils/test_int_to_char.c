#include "tests.h"

Test(zero_to_nine, test_zero_to_nine,
	 .description = "0-9 == 0-9", .timeout = 1.0)
{
	unsigned int n = 0;
	char out = '0';

	for (; n < 10; n++, out++)
	{
		cr_expect(eq(chr, int_to_char(n), out));
	}
}

Test(ten_to_thirtyfive, test_ten_to_thirtyfive,
	 .description = "10-35 == A-Z", .timeout = 1.0)
{
	unsigned int n = 10;
	char out = 'A';

	for (; n < 36; n++, out++)
	{
		cr_expect(eq(chr, int_to_char(n), out));
	}
}

Test(values_out_of_range, test_values_out_of_range,
	 .description = "values out of range should fail", .timeout = 1.0)
{
	cr_expect(zero(chr, int_to_char(36)));
	cr_expect(zero(chr, int_to_char(INT_MAX)));
	cr_expect(zero(chr, int_to_char(INT_MIN)));
	cr_expect(zero(chr, int_to_char(UINT_MAX)));
}
