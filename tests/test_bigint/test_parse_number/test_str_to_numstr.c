#include "tests.h"

struct invalid_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); };

TEST_F(invalid_inputs, test_NULL)
{
	EXPECT(str_to_numstr(NULL, 10, &processed) == 0);
	EXPECT(processed == 0);
}

TEST_F(invalid_inputs, test_null)
{
	EXPECT(str_to_numstr("\0001", 10, &processed) == 0);
	EXPECT(processed == 0);
}

struct invalid_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); };

TEST_F(invalid_inputs, test_all_nonvalid_chars)
{
	char c[2] = {SCHAR_MIN, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX; c[0]++)
	{
		if (!isalnum(c[0]))
			EXPECT(str_to_numstr(c, 36, NULL) == 0);
	}
}

TEST_F(invalid_inputs, test_all_nonvalid_chars2)
{
	char c[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};

	cr_redirect_stderr();
	for (; c[0] < SCHAR_MAX && c[1] > SCHAR_MIN; c[0]++, c[1]--)
	{
		if (isalnum(c[0]) || (c[0] == '-' && c[0] == '+' && isalnum(c[1])))
			continue;

		EXPECT(str_to_numstr(c, 36, NULL) == 0);
	}
}

TEST_F(invalid_inputs, test_leading_separators_signs)
{
	cr_redirect_stderr();
	/* Leading Separator. */
	EXPECT(str_to_numstr("---___", 10, &processed) == 0);
	EXPECT(processed == 3);
	EXPECT(str_to_numstr("+++___", 10, &processed) == 0);
	EXPECT(processed == 3);
	EXPECT(str_to_numstr("+++---__123", 10, &processed) == 0);
	EXPECT(processed == 6);
	EXPECT(str_to_numstr("-+-+-+-_0", 10, &processed) == 0);
	EXPECT(processed == 7);
	/* No valid characters. */
	EXPECT(str_to_numstr("---", 10, &processed) == 0);
	EXPECT(processed == 3);
	EXPECT(str_to_numstr("-+-+-+-", 10, &processed) == 0);
	EXPECT(processed == 7);
	EXPECT(str_to_numstr("+++", 10, NULL) == 0);
}

TEST_F(invalid_inputs, test_leading_underscore)
{
	EXPECT(str_to_numstr("_20", 10, &processed) == 0);
	EXPECT(processed == 0);
}

TEST_F(invalid_inputs, test_trailing_underscore)
{
	EXPECT(str_to_numstr("20_", 10, &processed) == 0);
	EXPECT(processed == 2);
}

TEST_F(invalid_inputs, test_neg_leading_underscore)
{
	cr_redirect_stderr();
	EXPECT(str_to_numstr("-_20", 10, &processed) == 0);
	EXPECT(processed == 1);
}

TEST_F(invalid_inputs, test_pos_leading_underscore)
{
	cr_redirect_stderr();
	EXPECT(str_to_numstr("+_20", 10, &processed) == 0);
	EXPECT(processed == 1);
}

struct invalid_chars_in_str
{
	bigint tau->num1;
	bigint tau->expected;
};

TEST_F_SETUP(invalid_chars_in_str) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_chars_in_str) { tau->output = bi_delete(tau->output); }

TEST_F(invalid_chars_in_str, test_valid_chars_with_dash_in_middle)
{
	cr_redirect_stderr();

	const char in[] = "123-567";
	char tau->expected[] = "123";

	tau->output = str_to_numstr(in, 10, &processed);
	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed == 3);
	EXPECT(tau->output->is_negative == false);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(invalid_chars_in_str, test_valid_chars_with_space_in_middle)
{

	const char in[] = "123 567";
	char tau->expected[] = "123";

	tau->output = str_to_numstr(in, 10, &processed);
	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(tau->expected) - 1 ==);
	EXPECT(tau->output->is_negative == false);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(invalid_chars_in_str, test_valid_chars_with_bracket_in_middle)
{

	const char in[] = "123(567)";
	char tau->expected[] = "123";

	tau->output = str_to_numstr(in, 10, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(tau->expected) - 1 ==);
	EXPECT(tau->output->is_negative == false);
	EXPECT(tau->output->str == tau->expected);
}

struct valid_inputs
{
	bigint tau->num1;
	bigint tau->expected;
};

TEST_F_SETUP(valid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(valid_inputs) { tau->output = bi_delete(tau->output); }

TEST_F(valid_inputs, test_decimals1)
{
	char c[2] = {'0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		tau->output = str_to_numstr(c, 10, &processed);

		EXPECT(tau->output->len == 1);
		EXPECT(processed == 1);
		EXPECT(tau->output->is_negative == false);
		EXPECT(tau->output->str == c);
		tau->output = free_numstr(tau->output);
	}
}

TEST_F(valid_inputs, test_uppercase_letters1)
{
	char c[2] = {'A', '\0'};

	for (; c[0] <= 'Z'; c[0]++)
	{
		tau->output = str_to_numstr(c, 36, &processed);

		EXPECT(tau->output->len == 1);
		EXPECT(processed == 1);
		EXPECT(tau->output->is_negative == false);
		EXPECT(tau->output->str == c);
		tau->output = free_numstr(tau->output);
	}
}

TEST_F(valid_inputs, test_lowercase_letters1)
{
	char c[2] = {'a', '\0'}, out[2] = {'A', '\0'};

	for (; c[0] <= 'z' && out[0] <= 'Z'; c[0]++, out[0]++)
	{
		tau->output = str_to_numstr(c, 36, &processed);

		EXPECT(tau->output->len == 1);
		EXPECT(processed == 1);
		EXPECT(tau->output->is_negative == false);
		EXPECT(tau->output->str == out);
		tau->output = free_numstr(tau->output);
	}
}

TEST_F(valid_inputs, test_decimals2)
{
	char c[3] = {'1', '0', '\0'};

	for (; c[0] <= '9'; c[0]++)
	{
		for (c[1] = '0'; c[1] <= '9'; c[1]++)
		{
			tau->output = str_to_numstr(c, 36, &processed);

			EXPECT(tau->output->len == 2);
			EXPECT(processed == 2);
			EXPECT(tau->output->is_negative == false);
			EXPECT(tau->output->str == c);
			tau->output = free_numstr(tau->output);
		}
	}
}

TEST_F(valid_inputs, test_uppercase_letters2)
{
	char c[3] = {'A', 'A', '\0'};

	for (; c[0] <= 'Z'; c[0]++)
	{
		for (c[1] = 'A'; c[1] <= 'Z'; c[1]++)
		{
			tau->output = str_to_numstr(c, 36, &processed);

			EXPECT(tau->output->len == 2);
			EXPECT(processed == 2);
			EXPECT(tau->output->is_negative == false);
			EXPECT(tau->output->str == c);
			tau->output = free_numstr(tau->output);
		}
	}
}

TEST_F(valid_inputs, test_lowercase_letters2)
{
	char c[3] = {'a', 'a', '\0'}, out[3] = {'A', 'A', '\0'};

	for (; c[0] <= 'z' && out[0] <= 'Z'; c[0]++, out[0]++)
	{
		for (c[1] = 'a', out[1] = 'A'; c[1] <= 'z' && out[1] <= 'Z';
		     c[1]++, out[1]++)
		{
			tau->output = str_to_numstr(c, 36, &processed);

			EXPECT(tau->output->len == 2);
			EXPECT(processed == 2);
			EXPECT(tau->output->is_negative == false);
			EXPECT(tau->output->str == out);
			tau->output = free_numstr(tau->output);
		}
	}
}

TEST_F(valid_inputs, test_0000000000000)
{

	const char in[] = "0000000000000";
	char tau->expected[] = "0";

	tau->output = str_to_numstr(in, 10, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(in) - 1 ==);
	EXPECT(tau->output->is_negative == false);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(valid_inputs, test_neg0000000000000)
{

	const char in[] = "-0000000000000";
	char tau->expected[] = "0";

	tau->output = str_to_numstr(in, 10, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(in) - 1 ==);
	EXPECT(tau->output->is_negative == true);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(valid_inputs, test_00000000000001)
{

	const char in[] = "00000000000001";
	char tau->expected[] = "1";

	tau->output = str_to_numstr(in, 10, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(in) - 1 ==);
	EXPECT(tau->output->is_negative == false);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(valid_inputs, test_neg00000000000001)
{

	const char in[] = "-00000000000001";
	char tau->expected[] = "1";

	tau->output = str_to_numstr(in, 10, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(in) - 1 ==);
	EXPECT(tau->output->is_negative == true);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(valid_inputs, test_mmneg10_000_000_000)
{

	const char in[] = "---10_000_000_000";
	char tau->expected[] = "10000000000";

	tau->output = str_to_numstr(in, 10, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(in) - 1 ==);
	EXPECT(tau->output->is_negative == true);
	EXPECT(tau->output->str == tau->expected);
}

TEST_F(valid_inputs, test_neg1)
{

	const char in[] = "--ZZZ_4Ar_YU8_012_qa9";
	char tau->expected[] = "ZZZ4ARYU8012QA9";

	tau->output = str_to_numstr(in, 36, &processed);

	EXPECT(tau->output->len, sizeof(tau->expected) - 1 ==);
	EXPECT(processed, sizeof(in) - 1 ==);
	EXPECT(tau->output->is_negative == false);
	EXPECT(tau->output->str == tau->expected);
}
