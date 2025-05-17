#include "tests.h"

struct invalid_inputs
{
	bigint base, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); }

TEST(invalid_inputs, test_null_pow_1)
{
	CHECK_PTR_EQ(bi_power(NULL, 1), NULL);
}

TEST(invalid_inputs, test_null_pow_0)
{
	CHECK_PTR_EQ(bi_power(NULL, 0), NULL);
}

TEST(invalid_inputs, test_null_pow_neg1)
{
	CHECK_PTR_EQ(bi_power(NULL, -1), NULL);
}

/* NaN */

TEST_F(invalid_inputs, test_NaN_pow_1)
{
	tau->output = bi_power(&tau->base, 1);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(invalid_inputs, test_NaN_pow_largenum)
{
	tau->output = bi_power(&tau->base, 238542068);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

struct simple_exponentiations
{
	bigint base, expected, *output;
};

TEST_F_SETUP(simple_exponentiations) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_exponentiations)
{
	tau->output = bi_delete(tau->output);
}

TEST_F(simple_exponentiations, test_123_pow_0)
{
	u_int bs[] = {123}, out[] = {1};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 0);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_1)
{
	u_int bs[] = {123}, out[] = {123};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 1);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_2)
{
	u_int bs[] = {123}, out[] = {15129};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 2);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_3)
{
	u_int bs[] = {123}, out[] = {1860867};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 3);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_4)
{
	u_int bs[] = {123}, out[] = {228886641};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 4);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_123_pow_5)
{
	u_int bs[] = {123}, out[] = {153056843, 28};

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 5);

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
	u_int bs[] = {234};
	u_int out[] = {138463744, 630382847, 425138196, 490057072, 42738656, 562352760, 295161161, 543519655, 265718800, 64620485, 975049337, 240021309, 215978840, 55782129, 849270075, 760996761, 32235035, 447626698, 534302852, 161083119, 30412973, 400786483, 992033896, 855034263, 435988368, 684563114, 266643638, 133173866, 673072223, 34504819, 342857813, 745043721, 798533122, 700144204, 470863810, 594437054, 430923488, 157091748, 450986271, 315274613, 838384114, 33278729, 133366133, 317251008, 441412309, 619366973, 282014902, 892514216, 906275551, 933202156, 866307837, 243896017, 610238855, 39603391, 272913065, 592191975, 999079521, 751657070, 241753884, 466580044, 868465256, 125117490, 825635666, 867236425, 252800494, 348107782, 931851, 794309767, 14743444, 766320565, 367541745, 432557403, 336829936, 249651988, 216412537, 487948097, 553749552, 103790680, 154383230, 537669816, 697724514, 825453593, 618993331, 13717548, 937161704, 677850779, 679885658, 229734360, 697004859, 938833278, 608403586, 18855403, 205656264, 373908591, 546690969, 765047235, 462433400, 637088006, 413981371, 515919479, 4353766, 416891820, 91823479, 776479816, 905684090, 602420390, 788973444, 583995007, 386478856, 359831547, 31927055, 690487036, 891942550, 852664769, 445808884, 892235228, 50712602, 512334281, 920209023, 950205994, 200783440, 842852295, 618665336, 406537103, 257391501, 130606};
	/* clang-format on */

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 477);

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
	u_int bs[] = {234};
	u_int out[] = {697007616, 658013101, 162202795, 73251460, 801345075, 657045283, 355942256, 871201480, 833741925, 164602412, 650273249, 755777576, 67826351, 932682058, 936889471, 240178765, 917296984, 610928359, 213259850, 81420064, 741164266, 237108231, 261918816, 24322661, 769693803, 981096105, 38216949, 885817392, 892756033, 349534593, 522160077, 468573133, 867025899, 245791842, 538671069, 242743507, 689940329, 26748175, 381062143, 331416069, 50951868, 176671784, 274749583, 901295397, 35807714, 169754602, 722630030, 538497733, 382131844, 776449123, 414860898, 652989656, 729447453, 637681316, 873137881, 207652620, 634848248, 734487857, 253451423, 223674185, 988937383, 571235276, 638935086, 381512860, 733925572, 610154307, 545711481, 22012290, 385664924, 627114334, 1};
	/* clang-format on */

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 266);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(simple_exponentiations, test_9847_pow_525)
{
	/* clang-format off */
	u_int bs[] = {9847};
	u_int out[] = {718406807, 838934696, 918927641, 248230681, 655163181, 252369778, 892071187, 987148570, 537595147, 62103025, 169106547, 192375617, 20472376, 793612401, 465844292, 42571378, 381338405, 612527666, 258164308, 852388587, 506704015, 652813760, 904575728, 721478894, 651842884, 296475346, 307716637, 467857491, 339523621, 327047075, 50859426, 685355539, 968707021, 846290406, 476099468, 69160989, 952276582, 287055838, 224462543, 350097400, 83319300, 116693572, 350238376, 684645490, 186270522, 77520798, 93928257, 630828309, 548045453, 156042083, 671776125, 538940746, 883986146, 213764396, 287923840, 759318137, 335025607, 898565562, 114774479, 921843543, 549032810, 857941948, 526855271, 603059463, 305968743, 649785641, 47525666, 503919000, 997303490, 802705143, 242511754, 849608418, 272147440, 768295344, 51389730, 648856809, 952315689, 713484724, 693242264, 458460078, 121633132, 710795130, 364800887, 80687518, 706292927, 601112921, 317631785, 490964386, 24423837, 316268976, 182265878, 257297900, 526966339, 552894485, 908893976, 192247960, 636177304, 655832957, 212843399, 132630029, 762201636, 331761709, 921437075, 541726321, 758723427, 480890669, 857438019, 787796065, 736557167, 874833834, 872194003, 472374310, 49115941, 54937124, 620615703, 391769116, 778797661, 798862546, 624052668, 522197497, 388270954, 660118131, 478571980, 532140993, 653833898, 309969279, 390183050, 368285800, 721855899, 639410646, 582363718, 924933234, 246860379, 123660495, 455592095, 94248930, 778653391, 771230182, 397553791, 871272898, 328293643, 359765195, 60806892, 372000770, 330595304, 256646973, 883188190, 277691227, 936937778, 657623925, 97455328, 494442185, 45399729, 394979082, 795739743, 161952307, 329664186, 395954323, 403820833, 282667305, 886582721, 657862402, 946270329, 14290273, 589828473, 648137367, 61151121, 310345289, 821521221, 186393458, 299577218, 727159778, 201235010, 824355686, 609215241, 709978621, 959782934, 703941748, 27788931, 263383146, 451511246, 992285958, 229859375, 760101064, 560504336, 32854974, 320401718, 72737537, 308023147, 868138334, 700323162, 985608681, 709360773, 743318025, 203970450, 805781108, 861490454, 949314398, 996086355, 806763826, 550159191, 887083510, 265191236, 892424952, 779685409, 663606664, 169547434, 68921953, 832225720, 323342237, 866842327, 800830329, 154955842, 460548120, 746037552, 37241474, 798241568, 448620814, 290848152, 391615870, 233937713, 325217023, 689259451, 669347600, 394917569, 305488388, 955399387, 626035997, 650554608, 129910036, 173366384, 79412336, 305187968};
	/* clang-format on */

	tau->base.len = sizeof(bs) / sizeof(*bs);
	tau->base.num = bs;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 525);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

struct negative_exponentiations
{
	bigint base, expected, *output;
};

TEST_F_SETUP(negative_exponentiations) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_exponentiations)
{
	tau->output = bi_delete(tau->output);
}

TEST_F(negative_exponentiations, test_neg123_pow_2)
{
	u_int bs[] = {123}, out[] = {15129};

	tau->base = (bigint){.len = sizeof(bs) / sizeof(*bs),
						 .is_negative = true,
						 .num = bs};
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_power(&tau->base, 2);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}

TEST_F(negative_exponentiations, test_neg123_pow_3)
{
	u_int bs[] = {123}, out[] = {1860867};

	tau->base = (bigint){.len = sizeof(bs) / sizeof(*bs),
						 .is_negative = true,
						 .num = bs};
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = true,
							 .num = out};
	tau->output = bi_power(&tau->base, 3);

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		tau->expected.len * sizeof(*(tau->expected.num))
	);
}
