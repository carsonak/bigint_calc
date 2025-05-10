#include "tests.h"

/**
 * capitalise - transform letters to upper case.
 * @c: the letter.
 * @args: is ignored.
 *
 * Return: the capitalised letter, 0 if digit, -1 otherwise.
 */
static char capitalise(const char c, void const *const args)
{
	(void)args;
	if (isalpha(c))
		return (toupper(c));

	if (isspace(c) || c == '_')
		return (c);

	if (isdigit(c))
		return (0);

	return (-1);
}

static len_type processed;

/**
 * setup - setup some variables.
 */
void setup(void)
{
	processed = 0;
}

/**
 * teardown - cleanup after tests.
 */
void teardown(void) {}

TestSuite(NULL_inputs, .init = setup, .fini = teardown);

Test(NULL_inputs, test_all_NULL_inputs,
	 .description = "filter_str(NULL, NULL, NULL, NULL) => NULL", .timeout = 1.0)
{
	cr_assert(zero(ptr, filter_str(NULL, NULL, NULL, NULL)));
}

Test(NULL_inputs, test_str_is_NULL,
	 .description = "filter_str(NULL, &processed, capitalise, NULL) => NULL",
	 .timeout = 1.0)
{
	cr_assert(zero(ptr, filter_str(NULL, &processed, capitalise, NULL)));
}

Test(NULL_inputs, test_processed_is_NULL,
	 .description = "filter_str(str, NULL, capitalise, NULL) => NULL",
	 .timeout = 1.0)
{
	const char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char expected[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char *out = filter_str(str, NULL, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	free(out);
}

Test(NULL_inputs, test_capitalise_is_NULL,
	 .description = "filter_str(str, &processed, NULL, NULL) => NULL",
	 .timeout = 1.0)
{
	const char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	cr_assert(zero(ptr, filter_str(str, &processed, NULL, NULL)));
}

Test(NULL_inputs, test_first_character_of_str_is_null,
	 .description = "in = \"\\0\"", .timeout = 1.0)
{
	const char in[] = "\0";
	char expected[] = "\0";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, 0));
	free(out);
}

TestSuite(filtering, .init = setup, .fini = teardown);

Test(filtering, test_string_has_exactly_one_character,
	 .description = "a => A",
	 .timeout = 1.0)
{
	const char in[] = "a";
	char expected[] = "A";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	free(out);
}

Test(filtering, test_all_chars_are_filtered,
	 .description = "lowercase => LOWERCASE",
	 .timeout = 1.0)
{
	const char in[] = "lowercase";
	char expected[] = "LOWERCASE";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	free(out);
}

Test(ignore_chars, test_characters_are_ignored,
	 .description = "'0Lorem 1Ipsum 2Dolor 3Sit 4Amet' => 'LOREM IPSUM DOLOR SIT AMET'",
	 .timeout = 1.0)
{
	const char in[] = "0Lorem 1Ipsum 2Dolor 3Sit 4Amet";
	char expected[] = "LOREM IPSUM DOLOR SIT AMET";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	free(out);
}

TestSuite(early_break, .init = setup, .fini = teardown);

Test(early_break, test_filtering_breaks_early_in_str,
	 .description = "special characters cause function to exit early",
	 .timeout = 1.0)
{
	const char in[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
	char expected[] = "LOREM IPSUM DOLOR SIT AMET";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(expected) - 1));
	free(out);
}

Test(early_break, test_first_character_in_str_causes_a_break,
	 .description = "in = '-The'", .timeout = 1.0)
{
	const char in[] = "-The";
	char expected[] = "\0";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, 0));
	free(out);
}

Test(no_change, test_string_with_all_ignored_characters,
	 .description = "'A_B_C' => 'A_B_C'", .timeout = 1.0)
{
	const char in[] = "A_B_C";
	char expected[] = "A_B_C";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	free(out);
}

Test(all_ignored, test_all_characters_in_string_are_ignored,
	 .description = "'0123456789' => ''", .timeout = 1.0)
{
	const char in[] = "0123456789";
	char expected[] = "";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	free(out);
}

Test(long_string, test_a_really_long_verse,
	 .description = "long string", .timeout = 1.0)
{
	const char in[] = "Look if you had one shot\nOr one spaghetti to seize every spaghetti you ever wanted\nIn one moment\nWould you capture it or just let it slip\n\n\nHis palms are spaghetti knees weak arms spaghetti\nThere_s vomit on his spaghetti already mom_s spaghetti\nHe_s nervous but on the surface he looks calm spaghetti\nTo drop spaghetti but he keeps on spaghetti\nWhat he wrote down the whole crowd goes spaghetti\nHe opens his mouth but spaghetti won_t come out\nHe_s choking how Everybody_s joking now\nThe spaghetti_s run out time_s up over  blaow\nSnap back to spaghetti oh  there goes gravity\nOh  there goes spaghetti he choked\nHe_s so mad but he won_t\nGive up spaghetti nope he won_t have it\nHe knows his whole back_s to these ropes\nIt don_t matter he_s dope\nHe knows that but he_s spaghetti\nHe_s so stagnant he knows\nWhen he goes back to this spaghetti that_s when it_s\nBack to the lab again yo this whole rhapsody\nHe better go capture this spaghetti and hope it don_t pass him\n\n\nYou better lose yourself in\nSpaghetti Spaghetti Spaghetti Spaghetti\nYou only get one shot do not miss your chance to blow\nThis spaghetti comes once in a lifetime\n\nYou better lose yourself in\nSpaghetti Spaghetti Spaghetti Spaghetti\nYou only get one shot do not miss your chance to blow\nThis spaghetti comes once in a lifetime\n\n\nThe spaghetti_s escaping through this hole that is gaping\nThis spaghetti is mine for the taking\nMake me spaghetti as we move toward a New World spaghetti\nA normal life is spaghetti but superspaghetti_s\nClose to spaghetti it only grows harder\nHomie grows spaghetti he blows it_s all over\nThese hoes is all spaghetti coast to coast spaghetti\nHe_s known as the Globetrotter\nLonely roads God only knows\nHe_s grown spaghetti from home he_s no father\nHe goes home and barely knows his own spaghetti\nBut hold your nose cause here goes the cold spaghetti\nThese hoes don_t want him no mo_ he_s cold spaghetti\nThey moved on to the next schmoe who flows\nHe nosedove and spaghetti And so the soap opera\nIs told it unfolds I suppose it_s old spaghetti\nBut the beat goes on dadadum dadum dadah\n";
	char expected[] = "LOOK IF YOU HAD ONE SHOT\nOR ONE SPAGHETTI TO SEIZE EVERY SPAGHETTI YOU EVER WANTED\nIN ONE MOMENT\nWOULD YOU CAPTURE IT OR JUST LET IT SLIP\n\n\nHIS PALMS ARE SPAGHETTI KNEES WEAK ARMS SPAGHETTI\nTHERE_S VOMIT ON HIS SPAGHETTI ALREADY MOM_S SPAGHETTI\nHE_S NERVOUS BUT ON THE SURFACE HE LOOKS CALM SPAGHETTI\nTO DROP SPAGHETTI BUT HE KEEPS ON SPAGHETTI\nWHAT HE WROTE DOWN THE WHOLE CROWD GOES SPAGHETTI\nHE OPENS HIS MOUTH BUT SPAGHETTI WON_T COME OUT\nHE_S CHOKING HOW EVERYBODY_S JOKING NOW\nTHE SPAGHETTI_S RUN OUT TIME_S UP OVER  BLAOW\nSNAP BACK TO SPAGHETTI OH  THERE GOES GRAVITY\nOH  THERE GOES SPAGHETTI HE CHOKED\nHE_S SO MAD BUT HE WON_T\nGIVE UP SPAGHETTI NOPE HE WON_T HAVE IT\nHE KNOWS HIS WHOLE BACK_S TO THESE ROPES\nIT DON_T MATTER HE_S DOPE\nHE KNOWS THAT BUT HE_S SPAGHETTI\nHE_S SO STAGNANT HE KNOWS\nWHEN HE GOES BACK TO THIS SPAGHETTI THAT_S WHEN IT_S\nBACK TO THE LAB AGAIN YO THIS WHOLE RHAPSODY\nHE BETTER GO CAPTURE THIS SPAGHETTI AND HOPE IT DON_T PASS HIM\n\n\nYOU BETTER LOSE YOURSELF IN\nSPAGHETTI SPAGHETTI SPAGHETTI SPAGHETTI\nYOU ONLY GET ONE SHOT DO NOT MISS YOUR CHANCE TO BLOW\nTHIS SPAGHETTI COMES ONCE IN A LIFETIME\n\nYOU BETTER LOSE YOURSELF IN\nSPAGHETTI SPAGHETTI SPAGHETTI SPAGHETTI\nYOU ONLY GET ONE SHOT DO NOT MISS YOUR CHANCE TO BLOW\nTHIS SPAGHETTI COMES ONCE IN A LIFETIME\n\n\nTHE SPAGHETTI_S ESCAPING THROUGH THIS HOLE THAT IS GAPING\nTHIS SPAGHETTI IS MINE FOR THE TAKING\nMAKE ME SPAGHETTI AS WE MOVE TOWARD A NEW WORLD SPAGHETTI\nA NORMAL LIFE IS SPAGHETTI BUT SUPERSPAGHETTI_S\nCLOSE TO SPAGHETTI IT ONLY GROWS HARDER\nHOMIE GROWS SPAGHETTI HE BLOWS IT_S ALL OVER\nTHESE HOES IS ALL SPAGHETTI COAST TO COAST SPAGHETTI\nHE_S KNOWN AS THE GLOBETROTTER\nLONELY ROADS GOD ONLY KNOWS\nHE_S GROWN SPAGHETTI FROM HOME HE_S NO FATHER\nHE GOES HOME AND BARELY KNOWS HIS OWN SPAGHETTI\nBUT HOLD YOUR NOSE CAUSE HERE GOES THE COLD SPAGHETTI\nTHESE HOES DON_T WANT HIM NO MO_ HE_S COLD SPAGHETTI\nTHEY MOVED ON TO THE NEXT SCHMOE WHO FLOWS\nHE NOSEDOVE AND SPAGHETTI AND SO THE SOAP OPERA\nIS TOLD IT UNFOLDS I SUPPOSE IT_S OLD SPAGHETTI\nBUT THE BEAT GOES ON DADADUM DADUM DADAH\n";

	char *out = filter_str(in, &processed, capitalise, NULL);

	cr_assert(eq(chr[sizeof(expected) - 1], out, expected));
	cr_assert(eq(sz, processed, sizeof(in) - 1));
	free(out);
}
