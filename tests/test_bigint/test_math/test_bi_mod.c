#include "test_math.h"

struct invalid_inputs
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(invalid_inputs) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(invalid_inputs) { tau->output = bi_delete(tau->output); };

TEST_F(invalid_inputs, test_null_modulo_null)
{
	CHECK_PTR_EQ(bi_modulo(NULL, NULL), NULL);
}

TEST_F(invalid_inputs, test_1_modulo_null)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;

	CHECK_PTR_EQ(bi_modulo(&(tau->num1), NULL), NULL);
}

TEST_F(invalid_inputs, test_null_modulo_1)
{
	u_int in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK_PTR_EQ(bi_modulo(NULL, &(tau->num2)), NULL);
}

TEST_F(invalid_inputs, test_0_modulo_null)
{
	u_int in1[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;

	CHECK_PTR_EQ(bi_modulo(&(tau->num1), NULL), NULL);
}

TEST_F(invalid_inputs, test_null_modulo_0)
{
	u_int in2[1] = {0};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK_PTR_EQ(bi_modulo(NULL, &(tau->num2)), NULL);
}

TEST_F(invalid_inputs, test_minus1_modulo_null)
{
	u_int in1[] = {1};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num1.is_negative = true;

	CHECK_PTR_EQ(bi_modulo(&(tau->num1), NULL), NULL);
}

TEST_F(invalid_inputs, test_null_modulo_minus1)
{
	u_int in2[] = {1};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->num2.is_negative = true;

	CHECK_PTR_EQ(bi_modulo(NULL, &(tau->num2)), NULL);
}

struct zero_len_arrays
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(zero_len_arrays) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(zero_len_arrays) { tau->output = bi_delete(tau->output); };

TEST_F(zero_len_arrays, test_null_modulo_largenum)
{
	u_int in2[] = {238542068, 232509426, 6086, 0, 0, 712000569, 99992175};
	u_int out[1] = {0};

	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct division_by_zero
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(division_by_zero) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(division_by_zero) { tau->output = bi_delete(tau->output); };

TEST_F(division_by_zero, test_0_mod_0)
{
	u_int in1[1] = {0}, in2[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK_PTR_EQ(bi_modulo(&(tau->num1), &(tau->num2)), NULL);
}

TEST_F(division_by_zero, test_1_mod_0)
{
	u_int in1[] = {1}, in2[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;

	CHECK_PTR_EQ(bi_modulo(&(tau->num1), &(tau->num2)), NULL);
}

struct negative_modulus
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(negative_modulus) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_modulus) { tau->output = bi_delete(tau->output); };

TEST_F(negative_modulus, test_neg_9107428777003_modulo_neg_809754437)
{
	u_int in1[] = {428777003, 9107}, in2[] = {809754437}, out[] = {120624064};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = true,
							 .num = out};

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(negative_modulus, test_neg_9107428777003_modulo_809754437)
{
	u_int in1[] = {428777003, 9107}, in2[] = {809754437}, out[] = {689130373};

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = true,
						 .num = in1};
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(negative_modulus, test_9107428777003_modulo_neg_809754437)
{
	u_int in1[] = {428777003, 9107}, in2[] = {809754437}, out[] = {689130373};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = true,
						 .num = in2};
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = true,
							 .num = out};

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct simple_modulus
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(simple_modulus) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_modulus) { tau->output = bi_delete(tau->output); };

TEST_F(simple_modulus, test_0_modulo_1)
{
	u_int in1[1] = {0}, in2[] = {1}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));
}

TEST_F(simple_modulus, test_1_modulo_1)
{
	u_int in1[] = {1}, in2[] = {1}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_modulus, test_1000000000_modulo_50000)
{
	u_int in1[] = {0, 1}, in2[] = {50000}, out[1] = {0};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_modulus, test_50000_modulo_100000000)
{
	u_int in1[] = {50000}, in2[] = {0, 1}, out[] = {50000};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(out) / sizeof(*out);
	tau->expected.num = out;

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(simple_modulus, test_longnum1_modulo_longnum2)
{
	u_int in1[] = {65000, 5, 45000, 0, 550005550, 2100};
	u_int in2[] = {0, 75006};
	u_int rem[] = {65000, 285};

	tau->num1.len = sizeof(in1) / sizeof(*in1);
	tau->num1.num = in1;
	tau->num2.len = sizeof(in2) / sizeof(*in2);
	tau->num2.num = in2;
	tau->expected.len = sizeof(rem) / sizeof(*rem);
	tau->expected.num = rem;
	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

struct large_modulus
{
	bigint num1, num2, expected, *output;
};

TEST_F_SETUP(large_modulus) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(large_modulus)
{
	tau->output = bi_delete(tau->output);
	free(tau->num1.num);
	free(tau->num2.num);
};

TEST_F(large_modulus, test_o1kb_modulo_o1kc)
{
	/* clang-format off */
	u_int in1[] = {645836236, 108893430, 836208119, 270771001, 537613755, 373039867, 833294108, 898839418, 608485063, 546188873, 452231917, 687296422, 111111152, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 111111111, 476411111, 5669047, 700035645, 893473278, 4493, 543630640, 324000, 568376700, 1394, 300000000, 378345892, 8, 734194700, 423768, 0, 445197384, 518101037, 109617878, 558156514, 983336821, 690076644, 693516675, 143226629, 76588580, 210759419, 973911560, 497333540, 14322213, 555048869, 815323758, 539167661, 458688023, 612425259, 624332483, 419554373, 679371075, 393235000, 236728315, 664373133, 150410609, 518660305, 359239326, 307363945, 571210805, 347785534, 563445710, 660409386, 228008317, 804462179, 233074148, 591937180, 160409198, 950235053, 962974549, 171857157, 523072994, 395079814, 321058488, 203652900, 126072395, 363107953, 630915068, 973080517, 612433363, 734409773, 377726669, 707225937, 627074076, 918808553, 941285428, 110569912, 430944738, 394600177, 990968620, 4498198, 265970467, 594659210, 881718792, 749254660, 489225907, 417635079, 182995180, 494838468, 16834009, 911110449, 948701330, 831191398, 792151714, 70940218, 627665839, 410612359, 202045867, 935804051, 937850141, 227018012, 991791152, 506752363, 406483698, 355792683, 47491630, 22359011, 919962818, 955165934, 399211808, 673319428, 319818160, 662785141, 175882430, 630322415, 735383534, 687650610, 46582669, 496678148, 491637987, 871575195, 116330034, 723314187, 385420689, 325094262, 12829142, 208703866, 120005690, 899921757, 77002169, 359486189, 753454258, 278911354, 694037307, 578519296, 494600786, 470779501, 768446187, 472669563, 716361937, 389127515, 976446119, 593192334, 617083917, 272976737, 418747631, 855714255, 226105187, 605917499, 696971445, 573117854, 99153257, 956645492, 728520292, 946389174, 245482697, 655465609, 470307816, 135416351, 729216591, 866144546, 888959338, 809402688, 527131609, 866613186, 419167460, 916600904, 633154349, 747140489, 315862951, 238759183, 148584074, 652667824, 161606397, 677427537, 590777629, 825244648, 835356427, 366982335, 748040826, 852122159, 4896, 900000000, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 999999999, 385820199, 167580321, 134092496, 277075937, 872169008, 64325722, 827080282, 277960712, 655352740, 163131699, 448213684, 277128786, 514326330, 933871396, 63154105, 745922799, 490015860, 252229607, 711956623, 591788040, 954758766, 467124003, 644524713, 690634208, 510828673, 629041620, 704066384, 468512842, 590156377, 509105852, 101803594, 777808979, 519622805, 972925209, 333284887, 366099858, 584319133, 851605740, 743738793, 912271659, 205902321, 306768696, 951511006, 853007337, 793747247, 858293618, 953587774, 433638706, 827128463, 10931106, 238240138, 541935268, 774418024, 534563619, 736369239, 336516542, 723202909, 72035747, 712282212, 121201039, 863656439, 738195137, 536402352, 988504044, 143816789, 991215483, 635637328, 796292638, 712577246, 870774981, 572335077, 92461651, 417842402, 465754445, 443219011, 441495263, 360383390, 613384414, 161967625, 730605620, 45166037, 501067481, 119816381, 482382428, 993423658, 444792073, 338718477, 357679842, 186546204, 725908514, 242243363, 998754918, 228221686, 517265463, 874612151, 181719511, 64245869, 179742910, 880842840, 733045974, 909570009, 761337985, 469116507, 501497182, 26189637, 141834727, 777673597, 739280728, 284499268, 848912007, 618450280, 855914917, 508218929, 64124951, 440691829, 852588078, 555626401, 202070759, 783604366, 202777713, 212620403, 857142928, 324004839, 349402718, 847565789, 52519590, 658472794, 842462660, 791141310, 195364150, 381999943, 261729562, 18229};
	u_int in2[] = {120720357, 397122822, 391212010, 378636564, 527381951, 445364023, 899885040, 831438167, 289912154, 386356373, 467962926, 817125772, 778707749, 515723350, 20924616, 454178424, 114657544, 634432190, 904414952, 143603833, 256133844, 201619676, 377306056, 810451660, 815010674, 281198163, 584823824, 739934236, 774447920, 423387184, 43576798, 141865462, 637259085, 182422433, 869987549, 632282216, 515172654, 118746121, 691817195, 100642458, 401797429, 748808428, 97330459, 859095700, 77613379, 824691165, 375014971, 270261896, 971418347, 287776735, 687392807, 72844992, 808489120, 176184502, 298559149, 515082189, 290641249, 784406918, 18525880, 595556264, 662020707, 137836043, 32027777, 282126204, 398571429, 183240048, 893494027, 908475657, 210325195, 41368093, 567494670, 78549187, 829867982, 939611731, 556446171, 260470617, 388185363, 898568742, 584926832, 930685864, 974259747, 196534676, 548532814, 760521395, 176790149, 995282978, 514569063, 129660418, 873623608, 869739334, 116299016, 931525892, 602082336, 842229732, 82941119, 584280220, 738070679, 549296857, 554682269, 129930289, 352088067, 846285574, 807033108, 924619988, 410518654, 524776378, 210726853, 338324773, 234371231, 345598217, 215913853, 40125185, 169659817, 614457144, 263430597, 839552963, 357693985, 358911228, 31559180, 973973025, 529509086, 130265707, 902771569, 939005219, 909614411, 481137671, 453755527, 315076922, 190488385, 858980849, 914750855, 893492457, 44932638, 537640698, 831905869, 686543771, 164968285, 32694655, 549006585, 907130711, 937753741, 473161761, 60159427, 193787354, 490610706, 977776760, 692607694, 6731141, 102078922, 359416565, 975892579, 663415745, 486552688, 501065002, 401362961, 293547871, 954030315, 717222449, 171804201, 933246578, 628660373, 388008411, 546067821, 318550725, 990682255, 492064339, 647433279, 137385221, 703102597, 102835383, 283915020, 951593059, 607687526, 836935402, 684651719, 23912707, 349733186, 901432221, 855504886, 181532375, 353916766, 945868802, 361242525, 362433248, 541955437, 67937107, 539323500, 323672831, 966437313, 515041060, 651866030, 535923932, 530736394, 457121080, 34778553, 656344571, 766040938, 922800831, 880446217, 23307414, 859193718, 316040919, 995023505, 796297454, 417185715, 452307299, 839507981, 32105848, 520365290, 312607239, 836310795, 763091506, 397308051, 361243336, 973440977, 737772666, 670722593, 362707407, 891880855, 294128542, 811056991, 743094473, 39460017, 899096862, 700449819, 26597046, 259465921, 88171879, 774925466, 948922590, 41763507, 818299518, 949483846, 901683400, 91111044, 894870133, 483119139, 879215171, 907094021, 962766583, 741061235, 120204586, 193580405, 293785014, 701801};
	u_int out[] = {924665483, 750983290, 453148054, 986040304, 164986354, 474238176, 415329533, 670194434, 652477846, 365427568, 860878235, 396957638, 766078512, 547467113, 617836051, 242924864, 233818787, 852794656, 698812520, 487167678, 491911641, 857783238, 254388353, 952383236, 949144867, 106498711, 201119901, 309276990, 640622509, 352163477, 446119850, 600680430, 126513302, 396585289, 980805856, 853283541, 163937595, 220575147, 174949394, 104545703, 446506461, 922508155, 262891967, 333987146, 987934355, 693184003, 627809690, 389943710, 461215845, 961396595, 35777825, 760113242, 56020856, 367928720, 126679107, 662982826, 634950814, 675736039, 375123144, 937918294, 711636698, 737704640, 471583518, 596851962, 760053157, 870213529, 129740950, 490803328, 286010125, 938349843, 178703505, 747510137, 133673433, 805555678, 309555664, 275008865, 289869166, 297855163, 14364687, 727969100, 875169639, 555030000, 154565581, 206151013, 711197148, 202433875, 475324188, 510237553, 521099681, 646028974, 31581979, 693401349, 60561793, 272572030, 336105990, 280407295, 925807770, 454397242, 423472626, 512258993, 654347980, 337064318, 760533116, 39817585, 343033350, 918327543, 722073616, 255378084, 765926125, 959934918, 48435172, 22622253, 219047971, 865214089, 247813909, 557598975, 915086120, 390468106, 872017366, 119820098, 27935731, 396768639, 250281557, 218586623, 616634730, 14872729, 820916992, 1883580, 804827276, 203418061, 816727575, 891004269, 437431673, 210108019, 72175855, 325143001, 798035830, 195012739, 961632005, 577297006, 324584949, 463170652, 407593150, 307540950, 838601695, 991514399, 634807707, 298437739, 706756585, 175642831, 86565941, 183076180, 358195905, 25169087, 451184335, 798602942, 308919404, 911802958, 497370734, 866456821, 218387279, 190105939, 175768231, 518447737, 355697056, 933332776, 849005322, 781966224, 843033128, 173736471, 340734490, 817550661, 639935062, 57409651, 961288843, 733862509, 428846774, 746753652, 582866952, 171147300, 537425823, 870956503, 187695166, 170338988, 568698680, 222284973, 931344869, 824071997, 638209354, 849382835, 959497367, 905173249, 957818061, 695655908, 920638400, 791884832, 352593552, 62942639, 197966684, 357771581, 202420660, 167065918, 968320021, 599930400, 648345490, 713230764, 381996562, 295730221, 382320633, 307771152, 264960367, 39652166, 424832833, 680060500, 721686478, 312920748, 230336538, 481869936, 954643973, 166882680, 106171404, 725410855, 880912591, 254884722, 130486347, 840155423, 903793277, 228083862, 275986546, 144766577, 37146143, 751195271, 47875140, 538110836, 137671644, 387254579, 259871272, 977532509, 133129339, 920473633, 5269482, 708111117, 656782971, 664618656, 595470};
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
	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(large_modulus, test_o500d_modulo_o500e)
{
	/* clang-format off */
	u_int in1[] = {555555555, 55555555, 0, 0, 0, 0, 222222, 0, 0, 0, 0, 0, 0, 0, 0, 888888888, 888888888, 888888888, 888888888, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 110000000, 111111111, 111111111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 770000000, 777777777, 777777777, 777777, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 999000000, 99999999};
	u_int in2[] = {470282653, 363502795, 428381283, 187482382, 467199043, 93190808, 0, 0, 0, 400000000, 133986336, 818672645, 328552829, 467844093, 238950876, 737842756, 975675668, 665546560, 147030781, 113541635, 672921659, 886614454, 888895933, 888888888, 888888888, 888888888, 268888888, 160980940, 318652713, 746086661, 442022266, 249623720, 646946071, 524660960, 229717732, 571794052, 616061544, 679629263, 171257724, 787077498, 157233507, 209246165, 541784240, 146575444, 344321901, 44149526, 436038339, 561338441, 16196762, 773060562, 104516603, 150106748, 411981638, 444444444, 444444444, 444444444, 444444444, 444444444, 444444444, 4};
	u_int out[] = {363796229, 279142378, 963129475, 770306954, 574844482, 433079883, 436687720, 890675428, 660367572, 380078604, 877690117, 509662379, 968745146, 566014198, 335059521, 260333066, 174292321, 507159533, 806247199, 187909493, 608325234, 17379762, 494051271, 526144266, 502655506, 804714576, 184468942, 425298081, 463364874, 735905411, 287749525, 449646228, 300319305, 872816580, 181076057, 352216647, 548954487, 523644473, 276039729, 985057301, 610810070, 739085734, 925843779, 54984645, 81918644, 978940315, 827013717, 33644622, 262738785, 606684968, 399932252, 440340698, 634452879, 450381884, 579934640, 701595622, 544540764, 147343261, 252726405, 4};
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
	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(large_modulus, test_o100a_modulo_o100f)
{
	/* clang-format off */
	u_int in1[] = {502884197, 643383279, 62, 0, 0, 0, 0, 222550000, 972222222, 999999999, 338499999, 112000333, 111111111, 28347111, 0, 0, 0, 289104938, 487298347, 832000092, 94, 284000000, 576231637, 28, 0, 57349857, 0, 0, 698750000};
	u_int in2[] = {0, 101000000, 555555555, 295555555, 9574, 0, 0, 684800000, 252449427, 2, 0, 94930300, 13103309, 99070958, 766};
	u_int out[] = {502884197, 528383279, 54469664, 289382844, 236557835, 876778715, 905030941, 233307056, 991024687, 674279893, 935667396, 976411137, 700189127, 670035691, 302};
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
	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}

TEST_F(large_modulus, test_o100f_modulo_o100a)
{
	/* clang-format off */
	u_int in1[] = {0, 101000000, 555555555, 295555555, 9574, 0, 0, 684800000, 252449427, 2, 0, 94930300, 13103309, 99070958, 766};
	u_int in2[] = {502884197, 643383279, 62, 0, 0, 0, 0, 222550000, 972222222, 999999999, 338499999, 112000333, 111111111, 28347111, 0, 0, 0, 289104938, 487298347, 832000092, 94, 284000000, 576231637, 28, 0, 57349857, 0, 0, 698750000};
	u_int out[] = {0, 101000000, 555555555, 295555555, 9574, 0, 0, 684800000, 252449427, 2, 0, 94930300, 13103309, 99070958, 766};
	/* clang-format on */

	tau->num1 = (bigint){.len = sizeof(in1) / sizeof(*in1),
						 .is_negative = false,
						 .num = in1};
	tau->num2 = (bigint){.len = sizeof(in2) / sizeof(*in2),
						 .is_negative = false,
						 .num = in2};
	tau->expected = (bigint){.len = sizeof(out) / sizeof(*out),
							 .is_negative = false,
							 .num = out};

	tau->output = bi_modulo(&(tau->num1), &(tau->num2));

	CHECK(tau->output->len == tau->expected.len);
	CHECK(tau->output->is_negative == tau->expected.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, tau->expected.num,
		sizeof(*(tau->expected.num)) * tau->expected.len
	);
}
