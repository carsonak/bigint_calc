#include "tests.h"

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * test_simpleAdittion - test a simple addition case
 */
void test_simpleAdittion(void)
{
	uint32_t result[2] = {1, 44};
	m_uint num1[2] = {1, 22};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(result, infiX_addition(num1, num1), 2, "22 + 22 = 44");
}

/**
 * test_carryOverAddition - addition that results in carry over
 */
void test_carryOverAddition(void)
{
	uint32_t result[3] = {2, 951394658, 1};
	m_uint num1[2] = {1, 975697329};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(result, infiX_addition(num1, num1), 2, "975,697,329 + 975,697,329 = 1,951,394,658");
}

/**
 * test_negativeAddPositive - adding a negative to a positive and vice versa
 */
void test_negativeAddPositive(void)
{
	uint32_t result[2] = {1, 735919929};
	m_uint num1[2] = {1, 975697329};
	m_uint num2[2] = {1, (NEGBIT_UI32 + 239777400)};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(result, infiX_addition(num1, num2), 2, "975,697,329 + -239,777,400 = 735,919,929");
	num2[1] = NEGBIT_UI32 + 239777400;
	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(result, infiX_addition(num2, num1), 2, "-239,777,400 + 975,697,329 = 735,919,929");
}

/**
 * test_negativeAddNegative - adding two negative numbers
 */
void test_negativeAddNegative(void)
{
	uint32_t result[2] = {1, (NEGBIT_UI32 + 479554800)};
	m_uint num1[2] = {1, (NEGBIT_UI32 + 239777400)};

	TEST_ASSERT_EQUAL_UINT32_ARRAY_MESSAGE(result, infiX_addition(num1, num1), 2, "-239,777,400 + -239,777,400 = -479554800");
}
