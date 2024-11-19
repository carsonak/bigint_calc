#include "tests.h"

Test(zero_to_nine, test_zero_to_nine,
	 .description = "0-9 == 0-9", .timeout = 1.0)
{
	char c = '0';
	int out = 0;

	for (; c <= '9'; ++c, ++out)
	{
		cr_expect(eq(int, char_to_int(c), out));
	}
}

Test(a_to_z, test_a_to_z,
	 .description = "a-z == 10-35", .timeout = 1.0)
{
	char c = 'a';
	int out = 10;

	for (; c <= 'z'; ++c, ++out)
	{
		cr_expect(eq(int, char_to_int(c), out));
	}
}

Test(A_to_Z, test_A_to_Z,
	 .description = "A-Z == 10-35", .timeout = 1.0)
{
	char c = 'A';
	int out = 10;

	for (; c <= 'Z'; ++c, ++out)
	{
		cr_expect(eq(int, char_to_int(c), out));
	}
}

Test(everything_else, test_everything_else,
	 .description = "non-alphanumeric should fail", .timeout = 1.0)
{
	int c = 0, out = -1;

	for (; c <= UCHAR_MAX; ++c)
	{
		if (isalnum(c))
			continue;

		cr_expect(eq(int, char_to_int(c), out));
	}
}
