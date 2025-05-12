#include "tests.h"

struct null_inputs
{
	bigint base, exponent, expected, *output;
};

TEST_F_SETUP(null_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(null_inputs) { tau->output = bi_delete(tau->output); }

TEST(null_inputs, test_null_pow_null)
{
	CHECK_PTR_EQ(bi_power(NULL, NULL), NULL);
}

TEST_F(null_inputs, test_1_pow_null)
{
	u_int bs[] = {1};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;

	CHECK_PTR_EQ(bi_power(&tau->base, NULL), NULL);
}

TEST_F(null_inputs, test_null_pow_1)
{
	u_int exp[] = {1};

	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;

	CHECK_PTR_EQ(bi_power(NULL, &tau->exponent), NULL);
}

TEST_F(null_inputs, test_0_pow_null)
{
	u_int bs[1] = {0};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;

	CHECK_PTR_EQ(bi_power(&tau->base, NULL), NULL);
}

TEST_F(null_inputs, test_null_pow_0)
{
	u_int exp[1] = {0};

	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;

	CHECK_PTR_EQ(bi_power(NULL, &tau->exponent), NULL);
}

TEST_F(null_inputs, test_neg1_pow_null)
{
	u_int bs[] = {1};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->base.is_negative = true;

	CHECK_PTR_EQ(bi_power(&tau->base, NULL), NULL);
}

TEST_F(null_inputs, test_null_pow_neg1)
{
	u_int exp[] = {1};

	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->exponent.is_negative = true;

	CHECK_PTR_EQ(bi_power(NULL, &tau->exponent), NULL);
}

struct zero_len_arrays
{
	bigint base, exponent, expected, *output;
};

TEST_F_SETUP(zero_len_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(zero_len_arrays) { tau->output = bi_delete(tau->output); }

TEST_F(zero_len_arrays, test_nullarray_pow_nullarray)
{
	u_int out[1] = {1};

	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == 0);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(zero_len_arrays, test_4490998_pow_nullarray)
{
	u_int bs[] = {4490998}, out[1] = {1};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == 0);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(zero_len_arrays, test_nullarray_pow_largenum)
{
	u_int exp[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175},
		  out[1] = {1};

	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == 0);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

struct simple_exponentiations
{
	bigint base, exponent, expected, *output;
};

TEST_F_SETUP(simple_exponentiations) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_exponentiations)
{
	tau->output = bi_delete(tau->output);
}

TEST_F(simple_exponentiations, test_123_pow_0)
{
	u_int bs[] = {123}, exp[] = {0}, out[] = {1};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_1)
{
	u_int bs[] = {123}, exp[] = {1}, out[] = {123};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_2)
{
	u_int bs[] = {123}, exp[] = {2}, out[] = {15129};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_3)
{
	u_int bs[] = {123}, exp[] = {3}, out[] = {1860867};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_4)
{
	u_int bs[] = {123}, exp[] = {4}, out[] = {228886641};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_5)
{
	u_int bs[] = {123}, exp[] = {5}, out[] = {153056843, 28};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_234_pow_477)
{
	/* clang-format off */
	u_int bs[] = {234}, exp[] = {477};
	u_int out[] = {138463744, 630382847, 425138196, 490057072, 42738656, 562352760, 295161161, 543519655, 265718800, 64620485, 975049337, 240021309, 215978840, 55782129, 849270075, 760996761, 32235035, 447626698, 534302852, 161083119, 30412973, 400786483, 992033896, 855034263, 435988368, 684563114, 266643638, 133173866, 673072223, 34504819, 342857813, 745043721, 798533122, 700144204, 470863810, 594437054, 430923488, 157091748, 450986271, 315274613, 838384114, 33278729, 133366133, 317251008, 441412309, 619366973, 282014902, 892514216, 906275551, 933202156, 866307837, 243896017, 610238855, 39603391, 272913065, 592191975, 999079521, 751657070, 241753884, 466580044, 868465256, 125117490, 825635666, 867236425, 252800494, 348107782, 931851, 794309767, 14743444, 766320565, 367541745, 432557403, 336829936, 249651988, 216412537, 487948097, 553749552, 103790680, 154383230, 537669816, 697724514, 825453593, 618993331, 13717548, 937161704, 677850779, 679885658, 229734360, 697004859, 938833278, 608403586, 18855403, 205656264, 373908591, 546690969, 765047235, 462433400, 637088006, 413981371, 515919479, 4353766, 416891820, 91823479, 776479816, 905684090, 602420390, 788973444, 583995007, 386478856, 359831547, 31927055, 690487036, 891942550, 852664769, 445808884, 892235228, 50712602, 512334281, 920209023, 950205994, 200783440, 842852295, 618665336, 406537103, 257391501, 130606};
	/* clang-format on */

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_234_pow_266)
{
	/* clang-format off */
	u_int bs[] = {234}, exp[] = {266};
	u_int out[] = {697007616, 658013101, 162202795, 73251460, 801345075, 657045283, 355942256, 871201480, 833741925, 164602412, 650273249, 755777576, 67826351, 932682058, 936889471, 240178765, 917296984, 610928359, 213259850, 81420064, 741164266, 237108231, 261918816, 24322661, 769693803, 981096105, 38216949, 885817392, 892756033, 349534593, 522160077, 468573133, 867025899, 245791842, 538671069, 242743507, 689940329, 26748175, 381062143, 331416069, 50951868, 176671784, 274749583, 901295397, 35807714, 169754602, 722630030, 538497733, 382131844, 776449123, 414860898, 652989656, 729447453, 637681316, 873137881, 207652620, 634848248, 734487857, 253451423, 223674185, 988937383, 571235276, 638935086, 381512860, 733925572, 610154307, 545711481, 22012290, 385664924, 627114334, 1};
	/* clang-format on */

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

struct negative_exponentiations
{
	bigint base, exponent, expected, *output;
};

TEST_F_SETUP(negative_exponentiations) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_exponentiations)
{
	tau->output = bi_delete(tau->output);
}

TEST_F(negative_exponentiations, test_neg123_pow_2)
{
	u_int bs[] = {123}, exp[] = {2}, out[] = {15129};

	tau->base = (bigint){.len = sizeof(bs) / sizeof(*bs),
						 .is_negative = true,
						 .num = bs};
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(negative_exponentiations, test_neg123_pow_3)
{
	u_int bs[] = {123}, exp[] = {3}, out[] = {1860867};

	tau->base = (bigint){.len = sizeof(bs) / sizeof(*bs),
						 .is_negative = true,
						 .num = bs};
	tau->exponent.len = sizeof(exp) / sizeof(*exp);
	tau->exponent.num = exp;
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = true,
							 .num = out};
	tau->output = bi_power(&tau->base, &tau->exponent);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}
