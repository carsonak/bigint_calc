#include "test_math.h"

/* ################################################################### */
/* ####################### invalid_inputs ############################ */
/* ################################################################### */

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

/* ################################################################### */
/* ################### simple_exponentiations ######################## */
/* ################################################################### */

struct simple_exponentiations
{
	bigint *restrict num1, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(simple_exponentiations) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(simple_exponentiations)
{
	bi_delete(tau->num1);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(simple_exponentiations, test_0_pow_0)
{
	const char expected[] = "1";

	tau->num1 = bi_new("0", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_1_pow_1)
{
	const char expected[] = "1";

	tau->num1 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_1_pow_longmax)
{
	const char expected[] = "1";

	tau->num1 = bi_new("1", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, LLONG_MAX);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_123_pow_0)
{
	const char expected[] = "1";

	tau->num1 = bi_new("123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 0);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_123_pow_1)
{
	const char expected[] = "123";

	tau->num1 = bi_new("123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 1);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_123_pow_2)
{
	const char expected[] = "15129";

	tau->num1 = bi_new("123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_123_pow_3)
{
	const char expected[] = "1860867";

	tau->num1 = bi_new("123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 3);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_123_pow_4)
{
	const char expected[] = "228886641";

	tau->num1 = bi_new("123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 4);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_123_pow_5)
{
	const char expected[] = "28153056843";

	tau->num1 = bi_new("123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 5);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_234_pow_477)
{
	/* clang-format off */
	const char expected[] = "130606257391501406537103618665336842852295200783440950205994920209023512334281050712602892235228445808884852664769891942550690487036031927055359831547386478856583995007788973444602420390905684090776479816091823479416891820004353766515919479413981371637088006462433400765047235546690969373908591205656264018855403608403586938833278697004859229734360679885658677850779937161704013717548618993331825453593697724514537669816154383230103790680553749552487948097216412537249651988336829936432557403367541745766320565014743444794309767000931851348107782252800494867236425825635666125117490868465256466580044241753884751657070999079521592191975272913065039603391610238855243896017866307837933202156906275551892514216282014902619366973441412309317251008133366133033278729838384114315274613450986271157091748430923488594437054470863810700144204798533122745043721342857813034504819673072223133173866266643638684563114435988368855034263992033896400786483030412973161083119534302852447626698032235035760996761849270075055782129215978840240021309975049337064620485265718800543519655295161161562352760042738656490057072425138196630382847138463744";
	/* clang-format on */

	tau->num1 = bi_new("234", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 477);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_234_pow_266)
{
	/* clang-format off */
	const char expected[] = "1627114334385664924022012290545711481610154307733925572381512860638935086571235276988937383223674185253451423734487857634848248207652620873137881637681316729447453652989656414860898776449123382131844538497733722630030169754602035807714901295397274749583176671784050951868331416069381062143026748175689940329242743507538671069245791842867025899468573133522160077349534593892756033885817392038216949981096105769693803024322661261918816237108231741164266081420064213259850610928359917296984240178765936889471932682058067826351755777576650273249164602412833741925871201480355942256657045283801345075073251460162202795658013101697007616";
	/* clang-format on */

	tau->num1 = bi_new("234", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 266);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(simple_exponentiations, test_9847_pow_525)
{
	/* clang-format off */
	const char expected[] = "305187968079412336173366384129910036650554608626035997955399387305488388394917569669347600689259451325217023233937713391615870290848152448620814798241568037241474746037552460548120154955842800830329866842327323342237832225720068921953169547434663606664779685409892424952265191236887083510550159191806763826996086355949314398861490454805781108203970450743318025709360773985608681700323162868138334308023147072737537320401718032854974560504336760101064229859375992285958451511246263383146027788931703941748959782934709978621609215241824355686201235010727159778299577218186393458821521221310345289061151121648137367589828473014290273946270329657862402886582721282667305403820833395954323329664186161952307795739743394979082045399729494442185097455328657623925936937778277691227883188190256646973330595304372000770060806892359765195328293643871272898397553791771230182778653391094248930455592095123660495246860379924933234582363718639410646721855899368285800390183050309969279653833898532140993478571980660118131388270954522197497624052668798862546778797661391769116620615703054937124049115941472374310872194003874833834736557167787796065857438019480890669758723427541726321921437075331761709762201636132630029212843399655832957636177304192247960908893976552894485526966339257297900182265878316268976024423837490964386317631785601112921706292927080687518364800887710795130121633132458460078693242264713484724952315689648856809051389730768295344272147440849608418242511754802705143997303490503919000047525666649785641305968743603059463526855271857941948549032810921843543114774479898565562335025607759318137287923840213764396883986146538940746671776125156042083548045453630828309093928257077520798186270522684645490350238376116693572083319300350097400224462543287055838952276582069160989476099468846290406968707021685355539050859426327047075339523621467857491307716637296475346651842884721478894904575728652813760506704015852388587258164308612527666381338405042571378465844292793612401020472376192375617169106547062103025537595147987148570892071187252369778655163181248230681918927641838934696718406807";
	/* clang-format on */

	tau->num1 = bi_new("9847", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 525);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

/* ################################################################### */
/* ################### negative_exponentiations ###################### */
/* ################################################################### */

struct negative_exponentiations
{
	bigint *restrict num1, *restrict output;
	char *restrict outstr;
};

TEST_F_SETUP(negative_exponentiations) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(negative_exponentiations)
{
	bi_delete(tau->num1);
	bi_delete(tau->output);
	xfree(tau->outstr);
}

TEST_F(negative_exponentiations, test_neg123_pow_2)
{
	const char expected[] = "15129";

	tau->num1 = bi_new("-123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 2);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}

TEST_F(negative_exponentiations, test_neg123_pow_3)
{
	const char expected[] = "-1860867";

	tau->num1 = bi_new("-123", 10, NULL);
	REQUIRE_PTR_NE(tau->num1, NULL);
	tau->output = bi_power(tau->num1, 3);
	REQUIRE_PTR_NE(tau->output, NULL);
	tau->outstr = bi_tostr(tau->output);
	REQUIRE_PTR_NE(tau->outstr, NULL);

	CHECK_STREQ(tau->outstr, expected);
}
