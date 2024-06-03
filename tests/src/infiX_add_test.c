#include "tests.h"

uint32_t blank1[10] = {0};
u4b_array num1 = {.len = 0, .is_negative = 0, .array = blank1};

uint32_t blank2[10] = {0};
u4b_array num2 = {.len = 0, .is_negative = 0, .array = blank2};

uint32_t blank3[10] = {0};
u4b_array expected = {.len = 0, .is_negative = 0, .array = blank3};

/**
 * trim_u4b_array - Dummy: trims zeros from end of an array
 * @arr: pointer to a u4b_array struct
 */
void trim_u4b_array(u4b_array *arr)
{
	if (!arr)
		return;

	while (!arr->array[arr->len - 1] && arr->len > 1)
		--arr->len;
}

/**
 * infiX_subtraction - Dummy
 * @n1: unused
 * @n2: unused
 *
 * Return: empty array, NULL on error
 */
u4b_array *infiX_subtraction(u4b_array *n1, u4b_array *n2)
{
	if (!n1 || !n2)
		return (NULL);

	return (alloc_u4b_array(1));
}

/**
 * setup - setups some variables for tests
 */
void setup(void)
{
	return;
}

/**
 * teardown - resets variables for tests.
 */
void teardown(void)
{
	num1.len = 0;
	num2.len = 0;
	expected.len = 0;
	memset(blank1, 0, sizeof(blank1));
	memset(blank2, 0, sizeof(blank1));
	memset(blank3, 0, sizeof(blank1));
}

TestSuite(simple_additions, .init = setup, .fini = teardown);

Test(simple_additions, test_0_plus_0,
	 .description = "0 + 0 = 0", .timeout = 0.5)
{
	uint32_t in1[] = {0}, out[] = {0};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num1);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_additions, test_1_plus_0,
	 .description = "1 + 0 = 1", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {0}, out[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_additions, test_0_plus_1,
	 .description = "0 + 1 = 1", .timeout = 0.5)
{
	uint32_t in1[] = {0}, in2[] = {1}, out[] = {1};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_additions, test_1_plus_1,
	 .description = "1 + 1 = 2", .timeout = 0.5)
{
	uint32_t in1[] = {1}, out[] = {2};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num1);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_additions, test_999999999_plus_1,
	 .description = "999,999,999 + 1 = 1,000,000,000", .timeout = 0.5)
{
	uint32_t in1[] = {1000000000 - 1}, in2[] = {1}, out[] = {0, 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(simple_additions, test_999999999_plus_999999999,
	 .description = "999,999,999 + 999,999,999 = 1,999,999,998",
	 .timeout = 0.5)
{
	uint32_t in1[] = {1000000000 - 1}, in2[] = {1000000000 - 1};
	uint32_t out[] = {999999998, 1};

	num1.len = sizeof(in1) / sizeof(*in1);
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

TestSuite(negative_additions, .init = setup, .fini = teardown);

Test(negative_additions, test_minus1_plus_minus1,
	 .description = "-1 + -1 = -2", .timeout = 0.5)
{
	uint32_t in1[] = {1}, in2[] = {1};
	uint32_t out[] = {2};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.is_negative = 1;
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.is_negative = 1;
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	expected.is_negative = 1;
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(eq(chr, output->is_negative, expected.is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

TestSuite(large_additions, .init = setup, .fini = teardown);

Test(large_additions, test_largenum1_plus_largenum2,
	 .description = "large1 + large2 = large3", .timeout = 0.5)
{
	uint32_t in1[] = {
		645836236,
		108893430,
		836208119,
		270771001,
		537613755,
		373039867,
		833294108,
		898839418,
		608485063,
		546188873,
		452231917,
		687296422,
		47476452,
		973845669,
		10374451,
		178785181,
		565141096,
		368215581,
		766449833,
		166756900,
		266296935,
		885801432,
		594190765,
		115602107,
		235409739,
		399179115,
		850675236,
		340648369,
		35579268,
		104749163,
		802235901,
		491996281,
		895516593,
		839921180,
		67331942,
		131981816,
		66278514,
		517588243,
		463032241,
		73538353,
		968765061,
		804658266,
		749667814,
		549163798,
		487157519,
		711633003,
		972331418,
		238542068,
		232509426,
		601282914,
		20870386,
		712000569,
		989992175,
		907700216,
		835948618,
		475345425,
		727891135,
		669403730,
		657851929,
		149460078,
		747077950,
		376844618,
		747266956,
		571636193,
		938912751,
		497644611,
		759319233,
		761708391,
		127297673,
		541874763,
		785571425,
		922610518,
		560591749,
		469697144,
		757311785,
		209915325,
		295664549,
		472852029,
		794638917,
		924548269,
		665546560,
		147030781,
		113541635,
		672921659,
		886614454,
		888895933,
		980940268,
		652713160,
		86661318,
		441916746,
		991660090,
		963315434,
		174714048,
		331586295,
		423875918,
		414858407,
		765266782,
		716160639,
		967742753,
		859077762,
		782524464,
		583535642,
		636698233,
		974804082,
		685212215,
		858201489,
		675803213,
		340924961,
		770759371,
		721690082,
		643257228,
		270802820,
		779607128,
		553527402,
		631316996,
		482136841,
		771287864,
		143263302,
		338713965,
		631541059,
		459227990,
		900158607,
		522296074,
		119566232,
		917880407,
		547587665,
		671240039,
		445247134,
		906342086,
		108286736,
		290416205,
		40663846,
		685128427,
		901563774,
		91058525,
		18035945,
		778089791,
		196228057,
		729252095,
		332848879,
		660998583,
		843191333,
		516057405,
		437387938,
		122716597,
		59023219,
		67686962,
		515110063,
		530073379,
		937472478,
		582936187,
		535877748,
		336387069,
		271284634,
		109311068,
		382401380,
		419352682,
		744180245,
		345636197,
		363692395,
		365165427,
		232029093,
		279405477,
		266065847,
		131084246,
		415079114,
		994319536,
		956238199,
		822926172,
		261,
	};
	uint32_t in2[] = {
		641945243,
		201854150,
		945526491,
		588113168,
		422870352,
		400027828,
		354131316,
		480925265,
		366384594,
		756651860,
		371009773,
		458461663,
		551681111,
		462833680,
		349616778,
		736870366,
		341533876,
		212381424,
		817120198,
		279193099,
		387442540,
		745368020,
		17956390,
		352189210,
		827292979,
		546656614,
		694960735,
		161299575,
		126946094,
		768192377,
		94511871,
		671285131,
		387504207,
		453076590,
		823542181,
		979958845,
		756289430,
		489039573,
		222402353,
		740510039,
		876371475,
		704920805,
		347966303,
		963059711,
		630699304,
		860282534,
		861738973,
		953072234,
		757507204,
		543460994,
		55998954,
		123911258,
		815547847,
		9353263,
		532748113,
		557418144,
		657649124,
		33796200,
		101643647,
		664228303,
		450577034,
		994356506,
		652298226,
		917712339,
		662382045,
		774713439,
		608723683,
		53246697,
		677980087,
		46254166,
		612192553,
		864487728,
		132108920,
		599737116,
		862767740,
		684688338,
		79278190,
		652585019,
		309195042,
		757798994,
		842976240,
		592825352,
		536380856,
		355273007,
		511686,
	};
	uint32_t out[] = {
		287781479,
		310747581,
		781734610,
		858884170,
		960484107,
		773067695,
		187425424,
		379764684,
		974869658,
		302840733,
		823241691,
		145758085,
		599157564,
		436679349,
		359991230,
		915655547,
		906674972,
		580597005,
		583570031,
		445950000,
		653739475,
		631169452,
		612147156,
		467791317,
		62702718,
		945835730,
		545635971,
		501947945,
		162525362,
		872941540,
		896747772,
		163281412,
		283020801,
		292997771,
		890874124,
		111940661,
		822567945,
		6627816,
		685434595,
		814048392,
		845136536,
		509579072,
		97634118,
		512223510,
		117856824,
		571915538,
		834070392,
		191614303,
		990016631,
		144743908,
		76869341,
		835911827,
		805540022,
		917053480,
		368696731,
		32763570,
		385540260,
		703199931,
		759495576,
		813688381,
		197654984,
		371201125,
		399565183,
		489348533,
		601294797,
		272358051,
		368042917,
		814955089,
		805277760,
		588128929,
		397763978,
		787098247,
		692700670,
		69434260,
		620079526,
		894603664,
		374942739,
		125437048,
		103833960,
		682347264,
		508522801,
		739856134,
		649922491,
		28194666,
		887126141,
		888895933,
		980940268,
		652713160,
		86661318,
		441916746,
		991660090,
		963315434,
		174714048,
		331586295,
		423875918,
		414858407,
		765266782,
		716160639,
		967742753,
		859077762,
		782524464,
		583535642,
		636698233,
		974804082,
		685212215,
		858201489,
		675803213,
		340924961,
		770759371,
		721690082,
		643257228,
		270802820,
		779607128,
		553527402,
		631316996,
		482136841,
		771287864,
		143263302,
		338713965,
		631541059,
		459227990,
		900158607,
		522296074,
		119566232,
		917880407,
		547587665,
		671240039,
		445247134,
		906342086,
		108286736,
		290416205,
		40663846,
		685128427,
		901563774,
		91058525,
		18035945,
		778089791,
		196228057,
		729252095,
		332848879,
		660998583,
		843191333,
		516057405,
		437387938,
		122716597,
		59023219,
		67686962,
		515110063,
		530073379,
		937472478,
		582936187,
		535877748,
		336387069,
		271284634,
		109311068,
		382401380,
		419352682,
		744180245,
		345636197,
		363692395,
		365165427,
		232029093,
		279405477,
		266065847,
		131084246,
		415079114,
		994319536,
		956238199,
		822926172,
		261,
	};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.array = check_calloc(sizeof(in1) / sizeof(*in1), sizeof(*num1.array));
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.array = check_calloc(sizeof(in2) / sizeof(*in2), sizeof(*num2.array));
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	expected.array = check_calloc(sizeof(out) / sizeof(*out), sizeof(*expected.array));
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}

Test(large_additions, test_medium_largenum1_plus_medium_largenum2,
	 .description = "medium_large1 + medium_large2 = medium_large3",
	 .timeout = 0.5)
{
	uint32_t in1[] = {966030813, 558415129, 215916370, 293454191, 307278912, 296694037, 276578519, 259353795, 189038184, 213740655, 379144106, 711552119, 390515874, 24999325, 433311074, 737617641, 631272976, 255418747, 566988069, 932347169, 371447913, 316036551, 823676490, 897860520, 70860951, 247735869, 452904526, 915518771, 630092085, 634989718, 156843447, 361193591, 679705492, 307915535, 881712128, 454623882, 137039507, 459673360, 837834147, 905194343, 99852740, 18779, 0, 93758200, 50367228, 473172540, 340597038, 282981867, 409332855, 765680784, 303317050, 566873784, 222697567, 237204420, 460712496, 609606469, 177325246, 940522297, 615445717, 445856160, 843054357, 498593376, 11612, 0, 967100000, 749106516, 714717857, 386634101, 30185493, 202001644, 743068356, 32117414, 18476997};
	uint32_t in2[] = {796130892, 369338127, 957556884, 938700736, 0, 0, 490000000, 757859194, 1567404, 649972209, 441671436, 195835757, 271126747, 690022879, 515717881, 557, 0, 0, 0, 0, 0, 0, 0, 644598900, 678291256, 634646638, 519091746, 159830334, 760018390, 43044724, 899790869, 967418363, 134695643, 463007216, 881712127, 454623882, 310507507, 431718657, 25000512, 875600, 19006021, 9460, 0, 93758200, 50367228, 839532540, 76083639, 0, 0, 678034000, 27693155};
	uint32_t out[] = {169899921, 189077002, 258359486, 354753454, 307278911, 296694037, 786578519, 501494600, 187470779, 563768446, 937472669, 515716361, 119389127, 334976446, 917593192, 737617083, 631272976, 255418747, 566988069, 932347169, 371447913, 316036551, 823676490, 253261620, 392569695, 613089230, 933812779, 755688436, 870073695, 591944993, 257052578, 393775227, 545009848, 844908319, 0, 0, 826532000, 27954702, 812833635, 904318743, 80846719, 9319, 0, 0, 0, 633640000, 264513398, 282981867, 409332855, 87646784, 275623895, 566873784, 222697567, 237204420, 460712496, 609606469, 177325246, 940522297, 615445717, 445856160, 843054357, 498593376, 11612, 0, 967100000, 749106516, 714717857, 386634101, 30185493, 202001644, 743068356, 32117414, 18476997};

	num1.len = sizeof(in1) / sizeof(*in1);
	num1.array = check_calloc(sizeof(in1) / sizeof(*in1), sizeof(*num1.array));
	memmove(num1.array, in1, sizeof(in1));

	num2.len = sizeof(in2) / sizeof(*in2);
	num2.array = check_calloc(sizeof(in2) / sizeof(*in2), sizeof(*num2.array));
	memmove(num2.array, in2, sizeof(in2));

	expected.len = sizeof(out) / sizeof(*out);
	expected.array = check_calloc(sizeof(out) / sizeof(*out), sizeof(*expected.array));
	memmove(expected.array, out, sizeof(out));

	u4b_array *output = infiX_addition(&num1, &num2);

	cr_expect(eq(sz, output->len, expected.len));
	cr_expect(zero(chr, output->is_negative));
	cr_expect(eq(u32[expected.len], output->array, expected.array));
	free_u4b_array(output);
}
