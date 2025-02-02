#include "tests.h"

bigint base = {.len = 0, .is_negative = false, .num = NULL};
bigint exponent = {.len = 0, .is_negative = false, .num = NULL};
bigint expected = {.len = 0, .is_negative = false, .num = NULL};

/**
 * setup - initialises variables for tests.
 */
void setup(void) {}

/**
 * teardown - resets variables for tests.
 */
void teardown(void)
{
	base.len = 0;
	base.is_negative = false;
	base.num = NULL;

	exponent.len = 0;
	exponent.is_negative = false;
	exponent.num = NULL;

	expected.len = 0;
	expected.is_negative = false;
	expected.num = NULL;
}

struct null_inputs
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(null_inputs)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(null_inputs) {}

TEST(null_inputs, test_null_pow_null)
{
	bigint *output = bi_power(NULL, NULL);

	CHECK(output == 0, "");
}

TEST(null_inputs, test_1_pow_null)
{
	u_int bs[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	bigint *output = bi_power(&base, NULL);

	CHECK(output == 0, "");
}

TEST(null_inputs, test_null_pow_1)
{
	u_int exp[] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	bigint *output = bi_power(NULL, &exponent);

	CHECK(output == 0, "");
}

TEST(null_inputs, test_0_pow_null)
{
	u_int bs[1] = {0};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	bigint *output = bi_power(&base, NULL);

	CHECK(output == 0, "");
}

TEST(null_inputs, test_null_pow_0)
{
	u_int exp[1] = {0};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	bigint *output = bi_power(NULL, &exponent);

	CHECK(output == 0, "");
}

TEST(null_inputs, test_minus1_pow_null)
{
	u_int bs[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	base.is_negative = true;
	bigint *output = bi_power(&base, NULL);

	CHECK(output == 0, "");
}

TEST(null_inputs, test_null_pow_minus1)
{
	u_int exp[] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	exponent.is_negative = true;
	bigint *output = bi_power(NULL, &exponent);

	CHECK(output == 0, "");
}

struct zero_len_arrays
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(zero_len_arrays)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(zero_len_arrays) {}

TEST(zero_len_arrays, test_nullarray_pow_nullarray)
{
	u_int out[1] = {1};

	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(output->is_negative == 0, "");
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(zero_len_arrays, test_4490998_pow_nullarray)
{
	u_int bs[] = {4490998}, out[1] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(output->is_negative == 0, "");
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(zero_len_arrays, test_nullarray_pow_largenum)
{
	u_int exp[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175},
		  out[1] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(output->is_negative == 0, "");
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

struct simple_exponentiations
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(simple_exponentiations)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(simple_exponentiations) {}

TEST(simple_exponentiations, test_123_pow_0)
{
	u_int bs[] = {123}, exp[] = {0}, out[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_123_pow_1)
{
	u_int bs[] = {123}, exp[] = {1}, out[] = {123};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_123_pow_2)
{
	u_int bs[] = {123}, exp[] = {2}, out[] = {15129};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_123_pow_3)
{
	u_int bs[] = {123}, exp[] = {3}, out[] = {1860867};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_123_pow_4)
{
	u_int bs[] = {123}, exp[] = {4}, out[] = {228886641};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_123_pow_5)
{
	u_int bs[] = {123}, exp[] = {5}, out[] = {153056843, 28};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_234_pow_477)
{
	/* clang-format off */
	u_int bs[] = {234}, exp[] = {477};
	u_int out[] = {138463744, 630382847, 425138196, 490057072, 42738656, 562352760, 295161161, 543519655, 265718800, 64620485, 975049337, 240021309, 215978840, 55782129, 849270075, 760996761, 32235035, 447626698, 534302852, 161083119, 30412973, 400786483, 992033896, 855034263, 435988368, 684563114, 266643638, 133173866, 673072223, 34504819, 342857813, 745043721, 798533122, 700144204, 470863810, 594437054, 430923488, 157091748, 450986271, 315274613, 838384114, 33278729, 133366133, 317251008, 441412309, 619366973, 282014902, 892514216, 906275551, 933202156, 866307837, 243896017, 610238855, 39603391, 272913065, 592191975, 999079521, 751657070, 241753884, 466580044, 868465256, 125117490, 825635666, 867236425, 252800494, 348107782, 931851, 794309767, 14743444, 766320565, 367541745, 432557403, 336829936, 249651988, 216412537, 487948097, 553749552, 103790680, 154383230, 537669816, 697724514, 825453593, 618993331, 13717548, 937161704, 677850779, 679885658, 229734360, 697004859, 938833278, 608403586, 18855403, 205656264, 373908591, 546690969, 765047235, 462433400, 637088006, 413981371, 515919479, 4353766, 416891820, 91823479, 776479816, 905684090, 602420390, 788973444, 583995007, 386478856, 359831547, 31927055, 690487036, 891942550, 852664769, 445808884, 892235228, 50712602, 512334281, 920209023, 950205994, 200783440, 842852295, 618665336, 406537103, 257391501, 130606};
	/* clang-format on */

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(simple_exponentiations, test_234_pow_266)
{
	/* clang-format off */
	u_int bs[] = {234}, exp[] = {266};
	u_int out[] = {697007616, 658013101, 162202795, 73251460, 801345075, 657045283, 355942256, 871201480, 833741925, 164602412, 650273249, 755777576, 67826351, 932682058, 936889471, 240178765, 917296984, 610928359, 213259850, 81420064, 741164266, 237108231, 261918816, 24322661, 769693803, 981096105, 38216949, 885817392, 892756033, 349534593, 522160077, 468573133, 867025899, 245791842, 538671069, 242743507, 689940329, 26748175, 381062143, 331416069, 50951868, 176671784, 274749583, 901295397, 35807714, 169754602, 722630030, 538497733, 382131844, 776449123, 414860898, 652989656, 729447453, 637681316, 873137881, 207652620, 634848248, 734487857, 253451423, 223674185, 988937383, 571235276, 638935086, 381512860, 733925572, 610154307, 545711481, 22012290, 385664924, 627114334, 1};
	/* clang-format on */

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

struct negative_exponentiations
{
	bigint num1;
	bigint expected;
};
TEST_F_SETUP(negative_exponentiations)
{
	tau->num1 = (bigint){0};
	tau->expected = (bigint){0};
}
TEST_F_TEARDOWN(negative_exponentiations) {}

TEST(negative_exponentiations, test_minus123_pow_2)
{
	u_int bs[] = {123}, exp[] = {2}, out[] = {15129};

	base = (bigint){
		.len = sizeof(bs) / sizeof(*bs), .is_negative = true, .num = bs};
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}

TEST(negative_exponentiations, test_minus123_pow_3)
{
	u_int bs[] = {123}, exp[] = {3}, out[] = {1860867};

	base = (bigint){
		.len = sizeof(bs) / sizeof(*bs), .is_negative = true, .num = bs};
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};
	bigint *output = bi_power(&base, &exponent);

	CHECK(output->len == expected.len, "length should be %zu", expected.len);
	CHECK(
		output->is_negative == expected.is_negative,
		"negative flag should be %u", expected.is_negative);
	CHECK_BUF_EQ(
		output->num, expected.num, expected.len * sizeof(*expected.num), "");
	output = bi_delete(output);
}
