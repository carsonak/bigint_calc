#include "tests.h"

bignum num1 = {.len = 0, .is_negative = false, .num = NULL};
bignum expected = {.len = 0, .is_negative = false, .num = NULL};

/**
 * bn_add_inplace - Dummy.
 * @n1: set to a dummy value.
 * @n2: set to a dummy value.
 *
 * Return: true always.
 */
bool bn_add_inplace(bignum *const n1, bignum *const n2)
{
	n1->num[0] = DUMMY_VALUE;
	n2->num[0] = DUMMY_VALUE;
	return (true);
}

/**
 * setup - initialises variables for tests.
 */
void setup(void) {}

/**
 * teardown - resets variables for tests.
 */
void teardown(void)
{
	num1 = (bignum){.len = 0, .is_negative = false, .num = NULL};
	expected = (bignum){.len = 0, .is_negative = false, .num = NULL};
}

TestSuite(null_inputs, .init = setup, .fini = teardown);

Test(null_inputs, test_null_minus_null,
	 .description = "NULL - 0 = NULL", .timeout = 2.0)
{
	cr_expect(zero(chr, bn_subint_inplace(NULL, 0)));
}

Test(null_inputs, test_null_minus_1,
	 .description = "NULL - 1 = NULL", .timeout = 2.0)
{
	cr_expect(zero(chr, bn_subint_inplace(NULL, 1)));
}

Test(null_inputs, test_null_minus_minus1,
	 .description = "NULL - -1 = NULL", .timeout = 2.0)
{
	cr_expect(zero(chr, bn_subint_inplace(NULL, -1)));
}

TestSuite(zero_len_arrays, .init = setup, .fini = teardown);

Test(zero_len_arrays, test_nullarray_minus_nullarray,
	 .description = "null_array - 0 = (invalid input)", .timeout = 2.0)
{
	cr_expect(zero(chr, bn_subint_inplace(&num1, 0)));
}

Test(zero_len_arrays, test_nullarray_minus_largenum,
	 .description = "null_array - 490998 = (invalid input)",
	 .timeout = 2.0)
{
	cr_expect(zero(chr, bn_subint_inplace(&num1, 490998)));
}

TestSuite(simple_subtractions, .init = setup, .fini = teardown);

Test(simple_subtractions, test_0_minus_0,
	 .description = "0 - 0 = 0", .timeout = 2.0)
{
	u_int in1[1] = {0}, out[1] = {0};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, 0);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(simple_subtractions, test_1_minus_0,
	 .description = "1 - 0 = 1", .timeout = 2.0)
{
	u_int in1[] = {1}, out[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, 0);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(simple_subtractions, test_0_minus_1,
	 .description = "0 - 1 = -1", .timeout = 2.0)
{
	u_int in1[1] = {0}, out[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bn_subint_inplace(&num1, 1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(simple_subtractions, test_1_minus_1,
	 .description = "1 - 1 = 0", .timeout = 2.0)
{
	u_int in1[] = {1}, out[1] = {0};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, 1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(simple_subtractions, test_MAX_VAL_u4b_minus_minus50000,
	 .description = "1,000000000 - 50,000 = 999,950,000",
	 .timeout = 2.0)
{
	u_int in1[] = {0, 1}, out[] = {999950000};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, 50000);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(simple_subtractions, test_50000_minus_100000000,
	 .description = "50,000 - 1,000000000 = -999950000",
	 .timeout = 2.0)
{
	u_int in1[] = {50000}, out[] = {999950000};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bn_subint_inplace(&num1, 1000000000);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(same_number_subtractions, .init = setup, .fini = teardown);

Test(same_number_subtractions, test_1000000001_minus_1000000001,
	 .description = "1000000001 - 1000000001 = 0", .timeout = 2.0)
{
	u_int in1[] = {1, 1}, out[] = {0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bn_subint_inplace(&num1, 1000000001);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(same_number_subtractions, test_longnum_minus_longnum,
	 .description = "1000000001000000001 - 1000000001000000001 = 0",
	 .timeout = 2.0)
{
	u_int in1[] = {1, 1, 1}, out[] = {0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = false, .num = in1};
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};

	bn_subint_inplace(&num1, 1000000001000000001);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(long_subtractions, .init = setup, .fini = teardown);

Test(long_subtractions, test_long9s_minus_1,
	 .description = "999999999,999999999,999999999,999999999,999999999 - 1 = "
					"999999999,999999999,999999999,999999999,999999998",
	 .timeout = 2.0)
{
	u_int in1[] = {BN_BASE - 1, BN_BASE - 1, BN_BASE - 1, BN_BASE - 1, BN_BASE - 1};
	u_int out[] = {BN_BASE - 2, BN_BASE - 1, BN_BASE - 1, BN_BASE - 1, BN_BASE - 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, 1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(zero(chr, num1.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(long_subtractions, test_1_minus_long9s,
	 .description = "1 - 999999999, 999999999 = -999999999, 999999998",
	 .timeout = 2.0)
{
	u_int in1[2] = {1};
	u_int out[2] = {BN_BASE - 2, BN_BASE - 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bn_subint_inplace(&num1, 999999999999999999);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(long_subtractions, test_4000000000678_minus_999999000,
	 .description = "4000,678 - 999999000 = 3999,1678",
	 .timeout = 2.0)
{
	u_int in1[] = {678, 4000}, out[] = {1678, 3999};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, 999999000);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(long_subtractions, test_999999000_minus_4000000000678,
	 .description = "999999000 - 4000,678 = -3999,1678",
	 .timeout = 2.0)
{
	u_int in1[2] = {999999000}, out[] = {1678, 3999};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};

	bn_subint_inplace(&num1, 4000000000678);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(negative_subtractions, .init = setup, .fini = teardown);

Test(negative_subtractions, test_minus1_minus_minus1,
	 .description = "-1 - -1 = 0", .timeout = 2.0)
{
	u_int in1[] = {1}, out[1] = {0};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, -1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(negative_subtractions, test_1_minus_minus1,
	 .description = "1 - -1 = 0 (does addition)", .timeout = 2.0)
{
	u_int in1[] = {1}, out[] = {DUMMY_VALUE};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};
	bn_subint_inplace(&num1, -1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(negative_subtractions, test_minus1_minus_1,
	 .description = "-1 - 1 = -2(does addition)", .timeout = 2.0)
{
	u_int in1[] = {1}, out[] = {DUMMY_VALUE};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};
	bn_subint_inplace(&num1, 1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(negative_subtractions, test_5_minus_minus1,
	 .description = "5 - -1 = 6 (does addition)", .timeout = 2.0)
{
	u_int in1[] = {5}, out[] = {DUMMY_VALUE};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = false, .num = out};
	bn_subint_inplace(&num1, -1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

Test(negative_subtractions, test_minus5_minus_1,
	 .description = "-5 - 1 = -6(does addition)", .timeout = 2.0)
{
	u_int in1[] = {6}, out[] = {DUMMY_VALUE};

	num1 = (bignum){
		.len = sizeof(in1) / sizeof(*in1), .is_negative = true, .num = in1};
	expected = (bignum){
		.len = sizeof(out) / sizeof(*out), .is_negative = true, .num = out};
	bn_subint_inplace(&num1, 1);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(eq(chr, num1.is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}

TestSuite(large_subtractions, .init = setup, .fini = teardown);

Test(large_subtractions, test_o1kb_minus_o1kc,
	 .description = "o_1k.b - LLONG_MAX = large3", .timeout = 2.0)
{
	u_int in1[342] = {645836236, 108893430, 836208119, 270771001, 537613755, 373039867, 833294108, 898839418, 608485063, 546188873, 452231917, 687296422, 111111152, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 476411111, 5669047, 700035645, 893473278, 4493, 543630640, 324000, 568376700, 1394, 300000000, 378345892, 8, 734194700, 423768, 0, 445197384, 518101037, 109617878, 558156514, 983336821, 690076644, 693516675, 143226629, 76588580, 210759419, 973911560, 497333540, 14322213, 555048869, 815323758, 539167661, 458688023, 612425259, 624332483, 419554373, 679371075, 393235000, 236728315, 664373133, 150410609, 518660305, 359239326, 307363945, 571210805, 347785534, 563445710, 660409386, 228008317, 804462179, 233074148, 591937180, 160409198, 950235053, 962974549, 171857157, 523072994, 395079814, 321058488, 203652900, 126072395, 363107953, 630915068, 973080517, 612433363, 734409773, 377726669, 707225937, 627074076, 918808553, 941285428, 110569912, 430944738, 394600177, 990968620, 4498198, 265970467, 594659210, 881718792, 749254660, 489225907, 417635079, 182995180, 494838468, 16834009, 911110449, 948701330, 831191398, 792151714, 70940218, 627665839, 410612359, 202045867, 935804051, 937850141, 227018012, 991791152, 506752363, 406483698, 355792683, 47491630, 22359011, 919962818, 955165934, 399211808, 673319428, 319818160, 662785141, 175882430, 630322415, 735383534, 687650610, 46582669, 496678148, 491637987, 871575195, 116330034, 723314187, 385420689, 325094262, 12829142, 208703866, 120005690, 899921757, 77002169, 359486189, 753454258, 278911354, 694037307, 578519296, 494600786, 470779501, 768446187, 472669563, 716361937, 389127515, 976446119, 593192334, 617083917, 272976737, 418747631, 855714255, 226105187, 605917499, 696971445, 573117854, 99153257, 956645492, 728520292, 946389174, 245482697, 655465609, 470307816, 135416351, 729216591, 866144546, 888959338, 809402688, 527131609, 866613186, 419167460, 916600904, 633154349, 747140489, 315862951, 238759183, 148584074, 652667824, 161606397, 677427537, 590777629, 825244648, 835356427, 366982335, 748040826, 852122159, 4896, 900000000, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 385820199, 167580321, 134092496, 277075937, 872169008, 64325722, 827080282, 277960712, 655352740, 163131699, 448213684, 277128786, 514326330, 933871396, 63154105, 745922799, 490015860, 252229607, 711956623, 591788040, 954758766, 467124003, 644524713, 690634208, 510828673, 629041620, 704066384, 468512842, 590156377, 509105852, 101803594, 777808979, 519622805, 972925209, 333284887, 366099858, 584319133, 851605740, 743738793, 912271659, 205902321, 306768696, 951511006, 853007337, 793747247, 858293618, 953587774, 433638706, 827128463, 10931106, 238240138, 541935268, 774418024, 534563619, 736369239, 336516542, 723202909, 72035747, 712282212, 121201039, 863656439, 738195137, 536402352, 988504044, 143816789, 991215483, 635637328, 796292638, 712577246, 870774981, 572335077, 92461651, 417842402, 465754445, 443219011, 441495263, 360383390, 613384414, 161967625, 730605620, 45166037, 501067481, 119816381, 482382428, 993423658, 444792073, 338718477, 357679842, 186546204, 725908514, 242243363, 998754918, 228221686, 517265463, 874612151, 181719511, 64245869, 179742910, 880842840, 733045974, 909570009, 761337985, 469116507, 501497182, 26189637, 141834727, 777673597, 739280728, 284499268, 848912007, 618450280, 855914917, 508218929, 64124951, 440691829, 852588078, 555626401, 202070759, 783604366, 202777713, 212620403, 857142928, 324004839, 349402718, 847565789, 52519590, 658472794, 842462660, 791141310, 195364150, 381999943, 261729562, 18229};
	u_int out[342] = {791060429, 885521393, 836208109, 270771001, 537613755, 373039867, 833294108, 898839418, 608485063, 546188873, 452231917, 687296422, 111111152, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 476411111, 5669047, 700035645, 893473278, 4493, 543630640, 324000, 568376700, 1394, 300000000, 378345892, 8, 734194700, 423768, 0, 445197384, 518101037, 109617878, 558156514, 983336821, 690076644, 693516675, 143226629, 76588580, 210759419, 973911560, 497333540, 14322213, 555048869, 815323758, 539167661, 458688023, 612425259, 624332483, 419554373, 679371075, 393235000, 236728315, 664373133, 150410609, 518660305, 359239326, 307363945, 571210805, 347785534, 563445710, 660409386, 228008317, 804462179, 233074148, 591937180, 160409198, 950235053, 962974549, 171857157, 523072994, 395079814, 321058488, 203652900, 126072395, 363107953, 630915068, 973080517, 612433363, 734409773, 377726669, 707225937, 627074076, 918808553, 941285428, 110569912, 430944738, 394600177, 990968620, 4498198, 265970467, 594659210, 881718792, 749254660, 489225907, 417635079, 182995180, 494838468, 16834009, 911110449, 948701330, 831191398, 792151714, 70940218, 627665839, 410612359, 202045867, 935804051, 937850141, 227018012, 991791152, 506752363, 406483698, 355792683, 47491630, 22359011, 919962818, 955165934, 399211808, 673319428, 319818160, 662785141, 175882430, 630322415, 735383534, 687650610, 46582669, 496678148, 491637987, 871575195, 116330034, 723314187, 385420689, 325094262, 12829142, 208703866, 120005690, 899921757, 77002169, 359486189, 753454258, 278911354, 694037307, 578519296, 494600786, 470779501, 768446187, 472669563, 716361937, 389127515, 976446119, 593192334, 617083917, 272976737, 418747631, 855714255, 226105187, 605917499, 696971445, 573117854, 99153257, 956645492, 728520292, 946389174, 245482697, 655465609, 470307816, 135416351, 729216591, 866144546, 888959338, 809402688, 527131609, 866613186, 419167460, 916600904, 633154349, 747140489, 315862951, 238759183, 148584074, 652667824, 161606397, 677427537, 590777629, 825244648, 835356427, 366982335, 748040826, 852122159, 4896, 900000000, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 385820199, 167580321, 134092496, 277075937, 872169008, 64325722, 827080282, 277960712, 655352740, 163131699, 448213684, 277128786, 514326330, 933871396, 63154105, 745922799, 490015860, 252229607, 711956623, 591788040, 954758766, 467124003, 644524713, 690634208, 510828673, 629041620, 704066384, 468512842, 590156377, 509105852, 101803594, 777808979, 519622805, 972925209, 333284887, 366099858, 584319133, 851605740, 743738793, 912271659, 205902321, 306768696, 951511006, 853007337, 793747247, 858293618, 953587774, 433638706, 827128463, 10931106, 238240138, 541935268, 774418024, 534563619, 736369239, 336516542, 723202909, 72035747, 712282212, 121201039, 863656439, 738195137, 536402352, 988504044, 143816789, 991215483, 635637328, 796292638, 712577246, 870774981, 572335077, 92461651, 417842402, 465754445, 443219011, 441495263, 360383390, 613384414, 161967625, 730605620, 45166037, 501067481, 119816381, 482382428, 993423658, 444792073, 338718477, 357679842, 186546204, 725908514, 242243363, 998754918, 228221686, 517265463, 874612151, 181719511, 64245869, 179742910, 880842840, 733045974, 909570009, 761337985, 469116507, 501497182, 26189637, 141834727, 777673597, 739280728, 284499268, 848912007, 618450280, 855914917, 508218929, 64124951, 440691829, 852588078, 555626401, 202070759, 783604366, 202777713, 212620403, 857142928, 324004839, 349402718, 847565789, 52519590, 658472794, 842462660, 791141310, 195364150, 381999943, 261729562, 18229};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.num = in1;
	expected.len = sizeof(out) / sizeof(*out);
	expected.num = out;

	bn_subint_inplace(&num1, LLONG_MAX);

	cr_expect(eq(sz, num1.len, expected.len));
	cr_expect(zero(chr, num1.is_negative));
	cr_expect(eq(u32[expected.len], num1.num, expected.num));
}
