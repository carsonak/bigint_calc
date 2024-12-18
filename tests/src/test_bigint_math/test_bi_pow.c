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

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_null_pow_null,
	 .description = "NULL ^ NULL = NULL", .timeout = 1.0)
{
	bigint *output = bi_power(NULL, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_1_pow_null,
	 .description = "1 ^ NULL = NULL", .timeout = 1.0)
{
	u_int bs[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	bigint *output = bi_power(&base, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_null_pow_1,
	 .description = "NULL ^ 1 = NULL", .timeout = 1.0)
{
	u_int exp[] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	bigint *output = bi_power(NULL, &exponent);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_0_pow_null,
	 .description = "0 ^ NULL = NULL", .timeout = 1.0)
{
	u_int bs[1] = {0};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	bigint *output = bi_power(&base, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_null_pow_0,
	 .description = "NULL ^ 0 = NULL", .timeout = 1.0)
{
	u_int exp[1] = {0};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	bigint *output = bi_power(NULL, &exponent);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_minus1_pow_null,
	 .description = "-1 ^ NULL = NULL", .timeout = 1.0)
{
	u_int bs[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	base.is_negative = true;
	bigint *output = bi_power(&base, NULL);

	cr_expect(zero(ptr, output));
}

Test(null_inputs, test_null_pow_minus1,
	 .description = "NULL ^ -1 = NULL", .timeout = 1.0)
{
	u_int exp[] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	exponent.is_negative = true;
	bigint *output = bi_power(NULL, &exponent);

	cr_expect(zero(ptr, output));
}

TestSuite(zero_len_arrays, .init = setup, .fini = teardown);

Test(zero_len_arrays, test_nullarray_pow_nullarray,
	 .description = "null_array ^ null_array = 1", .timeout = 1.0)
{
	u_int out[1] = {1};

	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(zero_len_arrays, test_4490998_pow_nullarray,
	 .description = "4,490,998 ^ null_array = 1", .timeout = 1.0)
{
	u_int bs[] = {4490998}, out[1] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(zero_len_arrays, test_nullarray_pow_largenum,
	 .description = "null_array ^ 99992175,712000569,0,0,6086,232509426,238542068 = 0",
	 .timeout = 1.0)
{
	u_int exp[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175}, out[1] = {1};

	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

TestSuite(simple_exponentiations, .init = setup, .fini = teardown);

Test(simple_exponentiations, test_123_pow_0, .description = "123 ^ 0", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {0}, out[] = {1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_123_pow_1, .description = "123 ^ 1", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {1}, out[] = {123};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_123_pow_2, .description = "123 ^ 2", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {2}, out[] = {15129};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_123_pow_3, .description = "123 ^ 3", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {3}, out[] = {1860867};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_123_pow_4, .description = "123 ^ 4", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {4}, out[] = {228886641};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_123_pow_5, .description = "123 ^ 5", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {5}, out[] = {153056843, 28};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_234_pow_477, .description = "234 ^ 477", .timeout = 1.0)
{
	u_int bs[] = {234}, exp[] = {477};
	u_int out[] = {138463744, 630382847, 425138196, 490057072, 42738656, 562352760, 295161161, 543519655, 265718800, 64620485, 975049337, 240021309, 215978840, 55782129, 849270075, 760996761, 32235035, 447626698, 534302852, 161083119, 30412973, 400786483, 992033896, 855034263, 435988368, 684563114, 266643638, 133173866, 673072223, 34504819, 342857813, 745043721, 798533122, 700144204, 470863810, 594437054, 430923488, 157091748, 450986271, 315274613, 838384114, 33278729, 133366133, 317251008, 441412309, 619366973, 282014902, 892514216, 906275551, 933202156, 866307837, 243896017, 610238855, 39603391, 272913065, 592191975, 999079521, 751657070, 241753884, 466580044, 868465256, 125117490, 825635666, 867236425, 252800494, 348107782, 931851, 794309767, 14743444, 766320565, 367541745, 432557403, 336829936, 249651988, 216412537, 487948097, 553749552, 103790680, 154383230, 537669816, 697724514, 825453593, 618993331, 13717548, 937161704, 677850779, 679885658, 229734360, 697004859, 938833278, 608403586, 18855403, 205656264, 373908591, 546690969, 765047235, 462433400, 637088006, 413981371, 515919479, 4353766, 416891820, 91823479, 776479816, 905684090, 602420390, 788973444, 583995007, 386478856, 359831547, 31927055, 690487036, 891942550, 852664769, 445808884, 892235228, 50712602, 512334281, 920209023, 950205994, 200783440, 842852295, 618665336, 406537103, 257391501, 130606};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(simple_exponentiations, test_234_pow_266, .description = "234 ^ 266", .timeout = 1.0)
{
	u_int bs[] = {234}, exp[] = {266};
	u_int out[] = {697007616, 658013101, 162202795, 73251460, 801345075, 657045283, 355942256, 871201480, 833741925, 164602412, 650273249, 755777576, 67826351, 932682058, 936889471, 240178765, 917296984, 610928359, 213259850, 81420064, 741164266, 237108231, 261918816, 24322661, 769693803, 981096105, 38216949, 885817392, 892756033, 349534593, 522160077, 468573133, 867025899, 245791842, 538671069, 242743507, 689940329, 26748175, 381062143, 331416069, 50951868, 176671784, 274749583, 901295397, 35807714, 169754602, 722630030, 538497733, 382131844, 776449123, 414860898, 652989656, 729447453, 637681316, 873137881, 207652620, 634848248, 734487857, 253451423, 223674185, 988937383, 571235276, 638935086, 381512860, 733925572, 610154307, 545711481, 22012290, 385664924, 627114334, 1};

	base.len = sizeof(bs) / sizeof(*bs);
	base.num = bs;
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

TestSuite(negative_exponentiations, .init = setup, .fini = teardown);

Test(negative_exponentiations, test_minus123_pow_2,
	 .description = "-123 ^ 2", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {2}, out[] = {15129};

	base = (bigint){
		.len = sizeof(bs) / sizeof(*bs), .is_negative = true, .num = bs};
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}

Test(negative_exponentiations, test_minus123_pow_3,
	 .description = "-123 ^ 3", .timeout = 1.0)
{
	u_int bs[] = {123}, exp[] = {3}, out[] = {1860867};

	base = (bigint){
		.len = sizeof(bs) / sizeof(*bs), .is_negative = true, .num = bs};
	exponent.len = sizeof(exp) / sizeof(*exp);
	exponent.num = exp;
	expected = (bigint){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};
	bigint *output = bi_power(&base, &exponent);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->num, expected.num));
	output = bi_free(output);
}
