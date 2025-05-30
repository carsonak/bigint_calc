#include "test_number_parsing.h"

TEST(invalid_inputs, test_NULL) { CHECK_PTR_EQ(_anybase_to_bi(NULL), NULL); }

TEST(invalid_inputs, test_base0)
{
	numstr input = {.len = 2, .base = 0, .str = "10"};
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_base1)
{
	numstr input = {.len = 2, .base = 1, .str = "10"};
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_base37)
{
	numstr input = {.len = 2, .base = 37, .str = "10"};
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_null_str)
{
	numstr input = {.len = 1, .base = 10, .str = NULL};
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_0_len)
{
	numstr input = {.len = 0, .base = 10, .str = "1"};

	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
}

TEST(invalid_inputs, test_all_nonvalid_char_str)
{
	char s[2] = {SCHAR_MIN, '\0'};
	numstr input = {.len = 4, .base = 10, .str = s};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX; s[0]++)
	{
		if (!isalnum(s[0]))
			CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	}
}

TEST(invalid_inputs, test_all_nonvalid_char_str1)
{
	char s[3] = {SCHAR_MIN, SCHAR_MIN, '\0'};
	numstr input = {.len = 4, .base = 10, .str = s};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX; s[0]++, s[1]++)
	{
		if (!isalnum(s[0]))
			CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	}
}

TEST(invalid_inputs, test_all_nonvalid_char_str1alt)
{
	char s[3] = {SCHAR_MIN, SCHAR_MAX, '\0'};
	numstr input = {.len = 4, .base = 10, .str = s};

	/* Error messages should be ignored. */
	for (; s[0] < SCHAR_MAX && s[1] > SCHAR_MIN; s[0]++, s[1]--)
	{
		if (!isalnum(s[0]) || !isalnum(s[1]))
			CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	}
}

TEST(invalid_inputs, test_all_invalid_char_str2)
{
	char s[] = {'\0', '\0', '\0', '\0'};
	numstr input = {.len = 4, .base = 10, .str = s};

	/* Error messages should be ignored. */
	s[0] = '+';
	for (s[1] = '0'; s[1] <= '9'; s[1]++)
	{
		for (s[2] = SCHAR_MIN; s[2] < SCHAR_MAX; s[2]++)
		{
			if (!isalnum(s[2]) && s[2] != '\0' && s[2] != '.')
				CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
		}
	}

	s[0] = '-';
	for (s[1] = '0'; s[1] <= '9'; s[1]++)
	{
		for (s[2] = SCHAR_MIN; s[2] < SCHAR_MAX; s[2]++)
		{
			if (!isalnum(s[2]) && s[2] != '\0' && s[2] != '.')
				CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
		}
	}
}

TEST(invalid_inputs, test_invalid_chars_mixed_with_valid_chars)
{
	numstr input = {.len = 7, .base = 10, .str = "123/567"};

	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123 567";
	/* Error messages should be ignored. */
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123\"567\"";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123+567";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123-567";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123*567";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123^567";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123(567)";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
	input.str = "123>567";
	CHECK_PTR_EQ(_anybase_to_bi(&input), NULL);
}

/* clang-format off */
const bigint ALL_BASES_EXPECTED = {.len = 18, .num = (u_int[]){346767800, 34560, 564500000, 740000003, 5600036, 64000000, 400054363, 670000032, 139456837, 0, 589230000, 834, 470000000, 376873419, 42, 400000000, 549338663, 169403018}};
/* clang-format on */

struct all_bases
{
	bigint *output;
};

TEST_F_SETUP(all_bases) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(all_bases) { tau->output = bi_delete(tau->output); }

TEST_F(all_bases, test_base36)
{
	/* clang-format off */
	char in_str[] = "8I8QTZC9O214OF1NACQ4HWVGY49XUO8Q8DP0Q40M6LFXM7FJ5PHQ9FPKS2EE3AQ175WMI2A9EHP85E7W8DIZS250JT5CWJL4V0X6VQGO";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 36, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base35)
{
	/* clang-format off */
	char in_str[] = "4ETQTML733WRPAII3HJHLQYHDW2JLNB0H29JTP9HITQTBW2IHJEI1127XNYKVBW6QM0GHYXUYWMNQTEHGDO5BKDT31WEY793N57HA7M2U";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 35, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base34)
{
	/* clang-format off */
	char in_str[] = "2M64VL4R8ETUUEJQGU2IM552JQ5CC8DK5F5Q965W2AATHT14QECGTIAP86OFK66L78VK2IBEFVO9JWL659KQGOXQ37TAR0T8QPB4110HOK";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 34, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base33)
{
	/* clang-format off */
	char in_str[] = "1RV9260GBJLR3GBHRNB21DWI6IKE7NQ2AN30GTSVI71P76VJPHA2G0V2USRJBV64T5PHUUIQVJEPGIO5B5WBM972GT2GSC68OD69SDNFI3F";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 33, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base32)
{
	/* clang-format off */
	char in_str[] = "1G6A3PAH9GD1MSEN2JPF2R5I4Q3URBO4URCSMTLALIVUF4TIT1SEL4MNN5OE0IRKA74KA80PV0VIHUEIH6RVA43372HMK8VBC1HP9BOR2GDO";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 32, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base31)
{
	/* clang-format off */
	char in_str[] = "1E00MP70OSBILF4BB8A032MDCEU64HE1AFIENHT1PRRQP3UJ4U2DSJRHNKQH8AS2DG54J564PQID6F384HEQS3LJI2SQRE55FAIJONLJG3RJC";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 31, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base30)
{
	/* clang-format off */
	char in_str[] = "1K2SMA6KTQ02CBA2J7F6FKIND4155Q85OG690GHCQAPQJEFT8FCTSI9T8SG1892GA64AS566PLKHND7CGHTCI721TE732JR6F7F935EK3H3IK0";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 30, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base29)
{
	/* clang-format off */
	char in_str[] = "296G9OK3D0D5PBPNDPKQ31Q4EFKSA5363Q584PN1GLAAD3GAMPEMPMHEGMF4P5CA3DRFFP8M3JMMDRN2I615H4KI35Q3BLI34H10NF84QI3K2MC";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 29, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base28)
{
	/* clang-format off */
	char in_str[] = "3Q10MBH44457D30C7LL7BGBI0KR1P0FD1CAQNBJMLP0FACGOFH2FOB1MIIBFLR262A8RQ7J0CK3QIH3RIMN2PBIDB4HRPFC632DFAFJ89AM4NLAG";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 28, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base27)
{
	/* clang-format off */
	char in_str[] = "86GC514L4G9G1HKGCNLD2L0BF8A2DK1J14A3Q70P6G57356J1E55646ACFF9D1FHBCBKNH24QCPG2BFJNBOG4D1CQ33G04HG7N8E2I5OH58J5B616";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 27, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base26)
{
	/* clang-format off */
	char in_str[] = "LIL4N7ILCPLKBOFK6N3A5JPLPPE5FPPPIM3056P75PCB8NP22OIL19CPCBME8968BH44AO02FCFEC5KI4KAO4CDD69E03KNCOE1OJJ2EFHJ0L221A0";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 26, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base25)
{
	/* clang-format off */
	char in_str[] = "2N1KO23O015E848I0N72GJNBG4A59F0INO32LMICK2FF9M8E6A838L1L6MJLHGCL41F0AEEMIL30850G42M53NB6K6IAO6B5L0EJ2I770091C94CI3C0";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 25, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base24)
{
	/* clang-format off */
	char in_str[] = "D7EIHLM5IE9GD27FJC4885GMIH308C840HK4L7D3D4G5F2IDBL44KJG8BM71ID4EJ5MB4MNE347AGI1KEH8E227DB326H2F60G70J0E31KMKI5KAD22D0";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 24, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base23)
{
	/* clang-format off */
	char in_str[] = "3BFED0J4J4FD62DM4FG481LMKFH9DJG2GC6I20M69HLC58LFLJKE3HCAB2GFH9B1GC080KD7GL9C82ACGJIJI1E2FC2ALKG13D5FB913D2H9BLH070GB48H";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 23, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base22)
{
	/* clang-format off */
	char in_str[] = "1857B9A652DH4A51C3594GJIEG54DB2EI6JF34IL5G6F20IH4522133HJ6FAJ3AABE4IF0HKH8BEJHHC13K6EBJ5E50CFF54E5H4JHFDI055H89238E2FF7G4";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 22, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base21)
{
	/* clang-format off */
	char in_str[] = "H85B0804G905HB6B0253JEEAH6BG0G51E7C3B0CDAD4D13HB2AC6I88K85JFHH92589C1B8B58K678119H6E872A43CE2ACKKH39EA72FJ5J3A2B76D1DHFD29";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 21, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base20)
{
	/* clang-format off */
	char in_str[] = "FIC4D6BAH3IGJ2AJACI194117G0EEE657FIC06GJ20J619BAJ78IIAA0770BIHCGC29392FIGCD25B3BBAAF4D14B6G2HAJF01AGGEE1CJJF74ACA37A05875JA0";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 20, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base19)
{
	/* clang-format off */
	char in_str[] = "154E85F35B00AB8CCFE528HEF6FADD1I5EB9D62E7C2GEBG323597CA13H48GA6ABFA08I8E16CD92EB2AG3DE88F5EA7ID08682G49751BDHBBE7HG5330971C6HII";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 19, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base18)
{
	/* clang-format off */
	char in_str[] = "3A8626A9FF5A5528AGFED385E4BE367FH450A4970A8H0AGA71A65AFCBD312F10FAF8CC27319BCH011318HBH07H797GDCAB2A1FG4E50AE00387GC6E532B5021FF6";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 18, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base17)
{
	/* clang-format off */
	char in_str[] = "11AG6B67C05AC872CA951008C15E5D4421F14D7A2F2E58D7AG640AB853G7G215A11E891D7G97472224G393DC2132B7AC124D3776CB7E579CAA495C05D486C2FG42F3";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 17, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base16)
{
	/* clang-format off */
	char in_str[] = "C0CA1E5514C1A1B71D714F2F16CB22687EDAF04F6D9CB76AAACBFE793B2E878EA92D7B970E04B7451C9452019F83F28F9D289B7F5106338A36A23EB606394AF1B141B8";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 16, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base15)
{
	/* clang-format off */
	char in_str[] = "14124CB4B4386E7C79A69CB57D28075B442B126A74A5232B678A84764B08B351DE31031B994E9AEB7C1B85D174CBC436944570025B606D7479410066A04CAB699A2C93DEA0";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 15, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base14)
{
	/* clang-format off */
	char in_str[] = "5C8AD8CB6D11AA97B66109A381C316A07800C17A37349648096D4964C33A18DD887B81ADA6528419D29B92A7C4CBB2B0D0B4942DA0AD951ACC9B38A5C34734C2CB7A5B4978172";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 14, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base13)
{
	/* clang-format off */
	char in_str[] = "68146A65B440783B4B5C117304924722459B41558A93B021619CACC1952016A039600A6BBBC171AA727C105B0B8915AA56A00B702C68C439BB0536C7385614929BCA673824CB73570";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 13, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base12)
{
	/* clang-format off */
	char in_str[] = "284495337401824B48B56BB1450859699A84695BA6199798360758AB027AA1559B3704B28AA1711457389027082940522663A49072A1181A9341A8578339581351AA335933B19250154A20";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 12, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base11)
{
	/* clang-format off */
	char in_str[] = "717483144711A8080517A2A031308813043365612047A500386A51016A84791920779A700A0239A409881685A07392A134503551073117071763757940945298450A140303573064A2A619918A4";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 11, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base10)
{
	/* clang-format off */
	char in_str[] = "169403018549338663400000000000000042376873419470000000000000834589230000000000000139456837670000032400054363064000000005600036740000003564500000000034560346767800";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 10, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base09)
{
	/* clang-format off */
	char in_str[] = "8217405034704540531582543783432700376271074661631141128230836535233166631465166136343516314046572412685562158427532376655386534431428103530148537768462605818168635366036";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 9, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base08)
{
	/* clang-format off */
	char in_str[] = "30062417125051406415561656123627426626211503755536023666634556652526277717116627207435244553671341601133505071121220031760374507635121155772420306342433242175330061624536154240670";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 8, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base07)
{
	/* clang-format off */
	char in_str[] = "44006051654212601631335162636004661505002310430101653325042541160100156500146066124110303022014355016000131024331603156134333150544510556604522602356540440521245222615024245564244055315421602";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 7, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base06)
{
	/* clang-format off */
	char in_str[] = "1230124245552013400201044023013514204204255251245404135350401242122141004204003410332353341123310541254213234132440222220314420111055234300214132225411205221152122130554402050031450520523133045100531051422440";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 6, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base05)
{
	/* clang-format off */
	char in_str[] = "243014044020344000110241304133300431202313443213104201014300033434403024142332240023030144213241120130313410141114234413231224104013000202424423341030013100031040242100343211140113320441121104100243402331212000014012214042233032200";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 5, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base04)
{
	/* clang-format off */
	char in_str[] = "3000302201321111011030012201231301311301103302330112302302021220133231223300103312312130231312222222302333321321032302322013203222210231132321130032001023131011013021101102000121332003330220332131022021231333110100120303202203122202033223120012032110223301230110012320";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 4, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base03)
{
	/* clang-format off */
	char in_str[] = "22020121110012001011210011121100121001122202121110212210111002210000102120022101002111202001201001011101010222021000221020121012021010012020201001112012012020011020101110120120100111001120122102110102202212122002011222110221121002102120201212102220121011111001110222010010121000011122121021212022112002200012220122012022201012102020001020";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 3, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}

TEST_F(all_bases, test_base02)
{
	/* clang-format off */
	char in_str[] = "11000000110010100001111001010101000101001100000110100001101101110001110101110001010011110010111100010110110010110010001001101000011111101101101011110000010011110110110110011100101101110110101010101010110010111111111001111001001110110010111010000111100011101010100100101101011110111001011100001110000001001011011101000101000111001001010001010010000000011001111110000011111100101000111110011101001010001001101101111111010100010000011000110011100010100011011010100010001111101011011000000110001110010100101011110001101100010100000110111000";
	/* clang-format on */
	numstr input = {.len = sizeof(in_str) - 1, .base = 2, .str = in_str};

	tau->output = _anybase_to_bi(&input);
	CHECK(tau->output->len == ALL_BASES_EXPECTED.len);
	CHECK(tau->output->is_negative == ALL_BASES_EXPECTED.is_negative);
	CHECK_BUF_EQ(
		tau->output->num, ALL_BASES_EXPECTED.num,
		sizeof(*ALL_BASES_EXPECTED.num) * ALL_BASES_EXPECTED.len
	);
}
