#include "test_number_parsing.h"

struct invalid_inputs
{
	numstr *output;
	len_ty processed;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = _numstr_free(tau->output); }

TEST_F(invalid_inputs, test_NULL)
{
	CHECK_PTR_EQ(_numstr_new(NULL, 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 0);
}

TEST_F(invalid_inputs, test_null)
{
	CHECK_PTR_EQ(_numstr_new("\0001", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 0);
}

TEST(invalid_inputs, test_all_nonvalid_chars)
{
	char s[2] = {SCHAR_MIN, '\0'};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX; s[0]++)
	{
		if (!isalnum(s[0]))
			CHECK_PTR_EQ(_numstr_new(s, 36, NULL), NULL);
	}
}

TEST(invalid_inputs, test_all_nonvalid_chars2)
{
	char s[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX && s[1] > SCHAR_MIN; s[0]++, s[1]--)
	{
		if (isalnum(s[0]) || (s[0] == '-' && s[0] == '+' && isalnum(s[1])))
			continue;

		CHECK_PTR_EQ(_numstr_new(s, 36, NULL), NULL);
	}
}

TEST_F(invalid_inputs, test_leading_separators_signs)
{
	/* Error messages should be ignored. */
	/* Leading Separator. */
	CHECK_PTR_EQ(_numstr_new("---___", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 3);
	CHECK_PTR_EQ(_numstr_new("+++___", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 3);
	CHECK_PTR_EQ(_numstr_new("+++---__123", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 6);
	CHECK_PTR_EQ(_numstr_new("-+-+-+-_0", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 7);
	/* No valid characters. */
	CHECK_PTR_EQ(_numstr_new("---", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 3);
	CHECK_PTR_EQ(_numstr_new("-+-+-+-", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 7);
	CHECK_PTR_EQ(_numstr_new("+++", 10, NULL), NULL);
}

TEST_F(invalid_inputs, test_leading_underscore)
{
	CHECK_PTR_EQ(_numstr_new("_20", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 0);
}

TEST_F(invalid_inputs, test_trailing_underscore)
{
	CHECK_PTR_EQ(_numstr_new("20_", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 2);
}

TEST_F(invalid_inputs, test_neg_leading_underscore)
{
	/* Error messages should be ignored. */
	CHECK_PTR_EQ(_numstr_new("-_20", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 1);
}

TEST_F(invalid_inputs, test_pos_leading_underscore)
{
	/* Error messages should be ignored. */
	CHECK_PTR_EQ(_numstr_new("+_20", 10, &tau->processed), NULL);
	REQUIRE(tau->processed == 1);
}

struct invalid_chars_in_str
{
	numstr *output;
	len_ty processed;
};

TEST_F_SETUP(invalid_chars_in_str) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_chars_in_str)
{
	tau->output = _numstr_free(tau->output);
}

TEST_F(invalid_chars_in_str, test_valid_chars_with_dash_in_middle)
{
	/* Error messages should be ignored. */

	const char input[] = "123-567";
	const char expected[] = "123";

	tau->output = _numstr_new(input, 10, &tau->processed);
	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == 3);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(invalid_chars_in_str, test_valid_chars_with_space_in_middle)
{

	const char input[] = "123 567";
	const char expected[] = "123";

	tau->output = _numstr_new(input, 10, &tau->processed);
	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(expected) - 1);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(invalid_chars_in_str, test_valid_chars_with_bracket_in_middle)
{

	const char input[] = "123(567)";
	const char expected[] = "123";

	tau->output = _numstr_new(input, 10, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(expected) - 1);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST(valid_chars, test_decimals1)
{
	char s[2] = {'0', '\0'};

	for (; s[0] <= '9'; s[0]++)
	{
		len_ty processed = 0;
		numstr *output = _numstr_new(s, 10, &processed);

		REQUIRE(output->len == 1);
		REQUIRE(processed == 1);
		REQUIRE(output->is_negative == false);
		REQUIRE_BUF_EQ(output->str, s, sizeof(s) - 1);
		output = _numstr_free(output);
	}
}

TEST(valid_chars, test_uppercase_letters1)
{
	char s[2] = {'A', '\0'};

	for (; s[0] <= 'Z'; s[0]++)
	{
		len_ty processed = 0;
		numstr *output = _numstr_new(s, 36, &processed);

		REQUIRE(output->len == 1);
		REQUIRE(processed == 1);
		REQUIRE(output->is_negative == false);
		REQUIRE_BUF_EQ(output->str, s, sizeof(s) - 1);
		output = _numstr_free(output);
	}
}

TEST(valid_chars, test_lowercase_letters1)
{
	char s[2] = {'a', '\0'}, out[2] = {'A', '\0'};

	for (; s[0] <= 'z' && out[0] <= 'Z'; s[0]++, out[0]++)
	{
		len_ty processed = 0;
		numstr *output = _numstr_new(s, 36, &processed);

		REQUIRE(output->len == 1);
		REQUIRE(processed == 1);
		REQUIRE(output->is_negative == false);
		REQUIRE_BUF_EQ(output->str, out, sizeof(out) - 1);
		output = _numstr_free(output);
	}
}

TEST(valid_chars, test_decimals2)
{
	char s[3] = {'1', '0', '\0'};

	for (; s[0] <= '9'; s[0]++)
	{
		for (s[1] = '0'; s[1] <= '9'; s[1]++)
		{
			len_ty processed = 0;
			numstr *output = _numstr_new(s, 36, &processed);

			REQUIRE(output->len == 2);
			REQUIRE(processed == 2);
			REQUIRE(output->is_negative == false);
			REQUIRE_BUF_EQ(output->str, s, sizeof(s) - 1);
			output = _numstr_free(output);
		}
	}
}

TEST(valid_chars, test_uppercase_letters2)
{
	char s[3] = {'A', 'A', '\0'};

	for (; s[0] <= 'Z'; s[0]++)
	{
		for (s[1] = 'A'; s[1] <= 'Z'; s[1]++)
		{
			len_ty processed = 0;
			numstr *output = _numstr_new(s, 36, &processed);

			REQUIRE(output->len == 2);
			REQUIRE(processed == 2);
			REQUIRE(output->is_negative == false);
			REQUIRE_BUF_EQ(output->str, s, sizeof(s) - 1);
			output = _numstr_free(output);
		}
	}
}

TEST(valid_chars, test_lowercase_letters2)
{
	char s[3] = {'a', 'a', '\0'}, out[3] = {'A', 'A', '\0'};

	for (; s[0] <= 'z' && out[0] <= 'Z'; s[0]++, out[0]++)
	{
		for (s[1] = 'a', out[1] = 'A'; s[1] <= 'z' && out[1] <= 'Z';
			 s[1]++, out[1]++)
		{
			len_ty processed = 0;
			numstr *output = _numstr_new(s, 36, &processed);

			REQUIRE(output->len == 2);
			REQUIRE(processed == 2);
			REQUIRE(output->is_negative == false);
			REQUIRE_BUF_EQ(output->str, out, sizeof(out) - 1);
			output = _numstr_free(output);
		}
	}
}

struct valid_inputs
{
	numstr *output;
	len_ty processed;
};

TEST_F_SETUP(valid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(valid_inputs) { tau->output = _numstr_free(tau->output); }

TEST_F(valid_inputs, test_0000000000000)
{

	const char input[] = "0000000000000";
	const char expected[] = "0";

	tau->output = _numstr_new(input, 10, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_neg0000000000000)
{

	const char input[] = "-0000000000000";
	const char expected[] = "0";

	tau->output = _numstr_new(input, 10, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == true);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_00000000000001)
{

	const char input[] = "00000000000001";
	const char expected[] = "1";

	tau->output = _numstr_new(input, 10, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_neg00000000000001)
{

	const char input[] = "-00000000000001";
	const char expected[] = "1";

	tau->output = _numstr_new(input, 10, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == true);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_nnneg10_000_000_000)
{

	const char input[] = "---10_000_000_000";
	const char expected[] = "10000000000";

	tau->output = _numstr_new(input, 10, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == true);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_neg_base36)
{

	const char input[] = "--ZZZ_4Ar_YU8_012_qa9";
	const char expected[] = "ZZZ4ARYU8012QA9";

	tau->output = _numstr_new(input, 36, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_base36_with_zeros)
{

	const char input[] = "ZZZ_4Ar_YU8_012_qa9_000_I00_000_0y0_ZSQ_000_004f";
	const char expected[] = "ZZZ4ARYU8012QA9000I000000Y0ZSQ000004F";

	tau->output = _numstr_new(input, 36, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == false);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}

TEST_F(valid_inputs, test_neg_base10_with_zeros)
{

	const char input[] =
		"-004_857_987_000_000_000_000_000_012_345_678_909_876_543_210";
	const char expected[] = "4857987000000000000000012345678909876543210";

	tau->output = _numstr_new(input, 36, &tau->processed);

	REQUIRE(tau->output->len == sizeof(expected) - 1);
	REQUIRE(tau->processed == sizeof(input) - 1);
	REQUIRE(tau->output->is_negative == true);
	CHECK_BUF_EQ(tau->output->str, expected, sizeof(expected) - 1);
}
