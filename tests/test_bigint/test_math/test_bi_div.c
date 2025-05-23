#include "test_math.h"

struct invalid_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); }

TEST_F(invalid_inputs, test_null_over_null)
{
	tau->output = bi_divide(NULL, NULL);

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(invalid_inputs, test_1_over_null)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->output = bi_divide(&(tau->num1), NULL);

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(invalid_inputs, test_null_over_1)
{
	u_int in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->output = bi_divide(NULL, &(tau->num2));

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(invalid_inputs, test_0_over_null)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->output = bi_divide(&(tau->num1), NULL);

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(invalid_inputs, test_null_over_0)
{
	u_int in2[1] = {0};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->output = bi_divide(NULL, &(tau->num2));

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(invalid_inputs, test_neg1_over_null)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num1.is_negative = true;
	tau->output = bi_divide(&(tau->num1), NULL);

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(invalid_inputs, test_null_over_neg1)
{
	u_int in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->num2.is_negative = true;
	tau->output = bi_divide(NULL, &(tau->num2));

	CHECK_PTR_EQ(tau->output, NULL);
}

struct zero_len_arrays
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(zero_len_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(zero_len_arrays) { tau->output = bi_delete(tau->output); }

TEST_F(zero_len_arrays, test_NaN_over_NaN)
{
	/* cr_redirect_stderr(); */
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(zero_len_arrays, test_4490998_over_NaN)
{
	u_int in1[] = {4490998};

	/* cr_redirect_stderr(); */
	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

TEST_F(zero_len_arrays, test_NaN_over_largenum)
{
	u_int in2[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_PTR_EQ(tau->output->num, NULL);
}

struct division_by_zero
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(division_by_zero) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(division_by_zero) { tau->output = bi_delete(tau->output); }

TEST_F(division_by_zero, test_0_over_0)
{
	u_int in1[1] = {0}, in2[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK_PTR_EQ(tau->output, NULL);
}

TEST_F(division_by_zero, test_1_over_0)
{
	u_int in1[] = {1}, in2[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK_PTR_EQ(tau->output, NULL);
}

struct simple_divisions
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(simple_divisions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_divisions) { tau->output = bi_delete(tau->output); }

TEST_F(simple_divisions, test_0_over_1)
{
	u_int in1[1] = {0}, in2[] = {1}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_divisions, test_1_over_1)
{
	u_int in1[] = {1}, in2[] = {1}, out[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_divisions, test_9723746_over_2938487)
{
	u_int in1[] = {9723746}, in2[] = {2938487}, out[] = {3};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_divisions, test_1000000000_over_50000)
{
	u_int in1[] = {0, 1}, in2[] = {50000}, out[] = {20000};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_divisions, test_50000_over_100000000)
{
	u_int in1[] = {50000}, in2[] = {0, 1}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_divisions, test_longnum1_over_longnum2)
{
	u_int in1[] = {65000, 5, 45000, 0, 550005550, 2100};
	u_int in2[] = {0, 75006};
	u_int out[] = {208916620, 865984055, 999893341, 28005092};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == 0);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_divisions, test_equal_over_equal)
{
	u_int in1[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175};
	u_int in2[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175};
	u_int out[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct negative_divisions
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(negative_divisions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_divisions) { tau->output = bi_delete(tau->output); }

TEST_F(negative_divisions, test_neg9107428777003_over_neg809754437)
{
	u_int in1[] = {428777003, 9107}, in2[] = {809754437}, out[] = {11247};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(negative_divisions, test_neg9107428777003_over_809754437)
{
	u_int in1[] = {428777003, 9107}, in2[] = {809754437}, out[] = {11248};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = false,
						 .num = in2};
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = true,
							 .num = out};

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(negative_divisions, test_9107428777003_over_neg809754437)
{
	u_int in1[] = {428777003, 9107}, in2[] = {809754437}, out[] = {11248};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = true,
							 .num = out};

	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct large_divisions
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(large_divisions) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(large_divisions)
{
	tau->output = bi_delete(tau->output);
	free(tau->num1.num);
	free(tau->num2.num);
}

TEST_F(large_divisions, test_o1kb_over_o1kc)
{
	/* clang-format off */
	u_int in1[] = {645836236, 108893430, 836208119, 270771001, 537613755, 373039867, 833294108, 898839418, 608485063, 546188873, 452231917, 687296422, 111111152, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 476411111, 5669047, 700035645, 893473278, 4493, 543630640, 324000, 568376700, 1394, 300000000, 378345892, 8, 734194700, 423768, 0, 445197384, 518101037, 109617878, 558156514, 983336821, 690076644, 693516675, 143226629, 76588580, 210759419, 973911560, 497333540, 14322213, 555048869, 815323758, 539167661, 458688023, 612425259, 624332483, 419554373, 679371075, 393235000, 236728315, 664373133, 150410609, 518660305, 359239326, 307363945, 571210805, 347785534, 563445710, 660409386, 228008317, 804462179, 233074148, 591937180, 160409198, 950235053, 962974549, 171857157, 523072994, 395079814, 321058488, 203652900, 126072395, 363107953, 630915068, 973080517, 612433363, 734409773, 377726669, 707225937, 627074076, 918808553, 941285428, 110569912, 430944738, 394600177, 990968620, 4498198, 265970467, 594659210, 881718792, 749254660, 489225907, 417635079, 182995180, 494838468, 16834009, 911110449, 948701330, 831191398, 792151714, 70940218, 627665839, 410612359, 202045867, 935804051, 937850141, 227018012, 991791152, 506752363, 406483698, 355792683, 47491630, 22359011, 919962818, 955165934, 399211808, 673319428, 319818160, 662785141, 175882430, 630322415, 735383534, 687650610, 46582669, 496678148, 491637987, 871575195, 116330034, 723314187, 385420689, 325094262, 12829142, 208703866, 120005690, 899921757, 77002169, 359486189, 753454258, 278911354, 694037307, 578519296, 494600786, 470779501, 768446187, 472669563, 716361937, 389127515, 976446119, 593192334, 617083917, 272976737, 418747631, 855714255, 226105187, 605917499, 696971445, 573117854, 99153257, 956645492, 728520292, 946389174, 245482697, 655465609, 470307816, 135416351, 729216591, 866144546, 888959338, 809402688, 527131609, 866613186, 419167460, 916600904, 633154349, 747140489, 315862951, 238759183, 148584074, 652667824, 161606397, 677427537, 590777629, 825244648, 835356427, 366982335, 748040826, 852122159, 4896, 900000000, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 385820199, 167580321, 134092496, 277075937, 872169008, 64325722, 827080282, 277960712, 655352740, 163131699, 448213684, 277128786, 514326330, 933871396, 63154105, 745922799, 490015860, 252229607, 711956623, 591788040, 954758766, 467124003, 644524713, 690634208, 510828673, 629041620, 704066384, 468512842, 590156377, 509105852, 101803594, 777808979, 519622805, 972925209, 333284887, 366099858, 584319133, 851605740, 743738793, 912271659, 205902321, 306768696, 951511006, 853007337, 793747247, 858293618, 953587774, 433638706, 827128463, 10931106, 238240138, 541935268, 774418024, 534563619, 736369239, 336516542, 723202909, 72035747, 712282212, 121201039, 863656439, 738195137, 536402352, 988504044, 143816789, 991215483, 635637328, 796292638, 712577246, 870774981, 572335077, 92461651, 417842402, 465754445, 443219011, 441495263, 360383390, 613384414, 161967625, 730605620, 45166037, 501067481, 119816381, 482382428, 993423658, 444792073, 338718477, 357679842, 186546204, 725908514, 242243363, 998754918, 228221686, 517265463, 874612151, 181719511, 64245869, 179742910, 880842840, 733045974, 909570009, 761337985, 469116507, 501497182, 26189637, 141834727, 777673597, 739280728, 284499268, 848912007, 618450280, 855914917, 508218929, 64124951, 440691829, 852588078, 555626401, 202070759, 783604366, 202777713, 212620403, 857142928, 324004839, 349402718, 847565789, 52519590, 658472794, 842462660, 791141310, 195364150, 381999943, 261729562, 18229};
	u_int in2[] = {120720357, 397122822, 391212010, 378636564, 527381951, 445364023, 899885040, 831438167, 289912154, 386356373, 467962926, 817125772, 778707749, 515723350, 20924616, 454178424, 114657544, 634432190, 904414952, 143603833, 256133844, 201619676, 377306056, 810451660, 815010674, 281198163, 584823824, 739934236, 774447920, 423387184, 43576798, 141865462, 637259085, 182422433, 869987549, 632282216, 515172654, 118746121, 691817195, 100642458, 401797429, 748808428, 97330459, 859095700, 77613379, 824691165, 375014971, 270261896, 971418347, 287776735, 687392807, 72844992, 808489120, 176184502, 298559149, 515082189, 290641249, 784406918, 18525880, 595556264, 662020707, 137836043, 32027777, 282126204, 398571429, 183240048, 893494027, 908475657, 210325195, 41368093, 567494670, 78549187, 829867982, 939611731, 556446171, 260470617, 388185363, 898568742, 584926832, 930685864, 974259747, 196534676, 548532814, 760521395, 176790149, 995282978, 514569063, 129660418, 873623608, 869739334, 116299016, 931525892, 602082336, 842229732, 82941119, 584280220, 738070679, 549296857, 554682269, 129930289, 352088067, 846285574, 807033108, 924619988, 410518654, 524776378, 210726853, 338324773, 234371231, 345598217, 215913853, 40125185, 169659817, 614457144, 263430597, 839552963, 357693985, 358911228, 31559180, 973973025, 529509086, 130265707, 902771569, 939005219, 909614411, 481137671, 453755527, 315076922, 190488385, 858980849, 914750855, 893492457, 44932638, 537640698, 831905869, 686543771, 164968285, 32694655, 549006585, 907130711, 937753741, 473161761, 60159427, 193787354, 490610706, 977776760, 692607694, 6731141, 102078922, 359416565, 975892579, 663415745, 486552688, 501065002, 401362961, 293547871, 954030315, 717222449, 171804201, 933246578, 628660373, 388008411, 546067821, 318550725, 990682255, 492064339, 647433279, 137385221, 703102597, 102835383, 283915020, 951593059, 607687526, 836935402, 684651719, 23912707, 349733186, 901432221, 855504886, 181532375, 353916766, 945868802, 361242525, 362433248, 541955437, 67937107, 539323500, 323672831, 966437313, 515041060, 651866030, 535923932, 530736394, 457121080, 34778553, 656344571, 766040938, 922800831, 880446217, 23307414, 859193718, 316040919, 995023505, 796297454, 417185715, 452307299, 839507981, 32105848, 520365290, 312607239, 836310795, 763091506, 397308051, 361243336, 973440977, 737772666, 670722593, 362707407, 891880855, 294128542, 811056991, 743094473, 39460017, 899096862, 700449819, 26597046, 259465921, 88171879, 774925466, 948922590, 41763507, 818299518, 949483846, 901683400, 91111044, 894870133, 483119139, 879215171, 907094021, 962766583, 741061235, 120204586, 193580405, 293785014, 701801};
	u_int out[] = {539037229, 551007555, 37410750, 953357327, 536760790, 766412595, 393281561, 860450642, 835850692, 53618879, 65892284, 355818216, 264458377, 549686035, 269784168, 15262813, 563471414, 275662089, 576789317, 371745470, 965369271, 110102852, 703741549, 741254328, 686959910, 603611024, 952582959, 36203065, 459073738, 925131187, 846423432, 307602240, 556589542, 253873211, 585961102, 662687946, 985916190, 21075717, 945816958, 16562243, 715353981, 312693843, 588775768, 20691418, 376852894, 851251583, 539224762, 145426353, 735783496, 234759697, 626979425, 609631678, 643988432, 402037206, 921779797, 886973088, 452594793, 136337576, 997167896, 9820330, 406687602, 313182840, 900324644, 794434349, 508575961, 946276063, 950598123, 870966156, 765500865, 48247002, 993555275, 419289051, 45182579, 470079979, 985847400, 427651623, 154268819, 810233239, 756454955, 54658389, 150421245, 918155740, 902115958, 103918578, 896652882, 147045386, 837644028, 709714739, 924511833, 504535099, 243496109, 945373742, 848312354, 835927813, 360776900, 703542584, 25974961};
	/* clang-format on */

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = malloc(sizeof(in1));
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = malloc(sizeof(in2));
	REQUIRE(tau->num1.num && tau->num2.num);
	memcpy(tau->num1.num, in1, sizeof(in1));
	memcpy(tau->num2.num, in2, sizeof(in2));

	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == 0);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(large_divisions, test_o500d_over_o500e)
{
	/* clang-format off */
	u_int in1[] = {555555555, 55555555, 0, 0, 0, 0, 222222, 0, 0, 0, 0, 0, 0, 0, 0, 888888888, 888888888, 888888888, 888888888, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110000000, 111111111, 111111111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 770000000, 777777777, 777777777, 777777, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 999000000, 99999999};
	u_int in2[] = {470282653, 363502795, 428381283, 187482382, 467199043, 93190808, 0, 0, 0, 400000000, 133986336, 818672645, 328552829, 467844093, 238950876, 737842756, 975675668, 665546560, 147030781, 113541635, 672921659, 886614454, 888895933, 888888888, 888888888, 888888888, 268888888, 160980940, 318652713, 746086661, 442022266, 249623720, 646946071, 524660960, 229717732, 571794052, 616061544, 679629263, 171257724, 787077498, 157233507, 209246165, 541784240, 146575444, 344321901, 44149526, 436038339, 561338441, 16196762, 773060562, 104516603, 150106748, 411981638, 444444444, 444444444, 444444444, 444444444, 444444444, 444444444, 4};
	u_int out[] = {164342, 0, 0, 0, 0, 999775000, 22499999};
	/* clang-format on */

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = malloc(sizeof(in1));
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = malloc(sizeof(in2));
	REQUIRE(tau->num1.num && tau->num2.num);
	memcpy(tau->num1.num, in1, sizeof(in1));
	memcpy(tau->num2.num, in2, sizeof(in2));

	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == 0);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(large_divisions, test_o100a_over_o100f)
{
	/* clang-format off */
	u_int in1[] = {502884197, 643383279, 62, 0, 0, 0, 0, 222550000, 972222222, 999999999, 338499999, 112000333, 111111111, 28347111, 0, 0, 0, 289104938, 487298347, 832000092, 94, 284000000, 576231637, 28, 0, 57349857, 0, 0, 698750000};
	u_int in2[] = {0, 101000000, 555555555, 295555555, 9574, 0, 0, 684800000, 252449427, 2, 0, 94930300, 13103309, 99070958, 766};
	u_int out[] = {467911615, 986420336, 456339159, 762062729, 530481552, 890502739, 598230926, 265770966, 42977542, 156122193, 174848149, 489493242, 12775684, 300963748, 912088};
	/* clang-format on */

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = malloc(sizeof(in1));
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = malloc(sizeof(in2));
	REQUIRE(tau->num1.num && tau->num2.num);
	memcpy(tau->num1.num, in1, sizeof(in1));
	memcpy(tau->num2.num, in2, sizeof(in2));

	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(large_divisions, test_o100f_over_o100a)
{
	/* clang-format off */
	u_int in1[] = {0, 101000000, 555555555, 295555555, 9574, 0, 0, 684800000, 252449427, 2, 0, 94930300, 13103309, 99070958, 766};
	u_int in2[] = {502884197, 643383279, 62, 0, 0, 0, 0, 222550000, 972222222, 999999999, 338499999, 112000333, 111111111, 28347111, 0, 0, 0, 289104938, 487298347, 832000092, 94, 284000000, 576231637, 28, 0, 57349857, 0, 0, 698750000};
	u_int out[] = {0};
	/* clang-format on */

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = malloc(sizeof(in1));
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = malloc(sizeof(in2));
	REQUIRE(tau->num1.num && tau->num2.num);
	memcpy(tau->num1.num, in1, sizeof(in1));
	memcpy(tau->num2.num, in2, sizeof(in2));

	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;
	tau->output = bi_divide(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}
