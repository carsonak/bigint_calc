/*!
 * @file
 * @brief methods for multiplying bigint types.
 */

#include <string.h>  // memset

#include "_bi_internals.h"
#include "bigint.h"

/*!
 * @brief a `bigint` split into its most significant digits and least
 * significant digits.
 *
 * a `bi_split` can be reconstituted into a `bigint` (x) with
 * the formula:
 * ```C
 * if (lo > 0)
 *    x = (hi * BIGINT_BASE ^ lo.len) + lo
 * else
 *    x = hi
 * ```
 */
typedef struct bigint_karatsuba_split
{
	/*! the most significant digits of the `bigint`. */
	bigint hi;
	/*! the least significant digits of the `bigint`. */
	bigint lo;
} bi_split;

static bigint *long_multiply(
	const bigint *const restrict n1, const bigint *const restrict n2
) _nonnull;
static bi_split bi_split_at(const bigint *const restrict n, const len_ty i);
static bigint *karatsuba_multiply(
	const bigint *const restrict n1, const bigint *const restrict n2
);
static bigint *multiply_negatives(
	bigint *const restrict n1, bigint *const restrict n2
) _nonnull;

/*!
 * @brief multiply two `bigint`s.
 * @private @memberof bigint
 *
 * This function only multiplies unsigned numbers.
 *
 * @param[in] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to result, NULL on failure.
 */
static bigint *
long_multiply(const bigint *const restrict n1, const bigint *const restrict n2)
{
	// Multiplication by zero.
	if (bi_iszero(n1) || bi_iszero(n2))
		return (_bi_alloc(1));

	len_ty n2_i = 0;
	// Length of product = length of n1 + length of n2
	bigint *restrict product = _bi_resize(NULL, n1->len + n2->len);
	bigint *const restrict current_mul = _bi_alloc(n1->len + n2->len);

	if (!product || !current_mul)
		goto cleanup;

	product->len = 1;
	// Multiply every "digit" in `n2` with `n1`.
	for (n2_i = 0; n2_i < n2->len; ++n2_i)
	{
		len_ty n1_i = 0;
		ldigit_ty byt_prod = 0;

		// Skip multiplication by zero.
		if (n2->num[n2_i] == 0)
		{
			// Ensure least significant "digits" for the next iteration are
			// set to 0.
			current_mul->num[n2_i] = 0;
			continue;
		}

		// Length of current_mul =
		// length of n1 + (number of "digits" upto n2_i)
		current_mul->len = n1->len + (n2_i + 1);
		for (n1_i = 0; n1_i < n1->len; ++n1_i)
		{
			byt_prod += (ldigit_ty)n2->num[n2_i] * n1->num[n1_i];
			current_mul->num[n2_i + n1_i] = byt_prod % BIGINT_BASE;
			byt_prod /= BIGINT_BASE;
		}

		current_mul->num[n2_i + n1_i] = byt_prod;
		bi_iadd(product, current_mul);  // inplace addition should not fail.
		// Ensure least significant "digits" for the next iteration are set
		// to 0.
		current_mul->num[n2_i] = 0;
	}

	if (n2_i < n2->len)
	{
cleanup:
		product = _bi_free(product);
	}

	_bi_free(current_mul);
	return (product);
}

/*! zero as a `bigint`. */
static const bigint zero = {
	.len = 1, .is_negative = false, .num = (digit_ty[1]){0}
};

/*!
 * @brief split the "digits" of a `bigint` at the given index.
 * @public @memberof bi_split
 *
 * @param[in] n pointer to the `bigint`.
 * @param[in] i a positive integer indicating index to split at.
 *
 * @return a `bi_split` with both the higher digits and lower ones.
 */
static bi_split bi_split_at(const bigint *const restrict n, const len_ty i)
{
	bi_split split = {0};

	if (i < 0)
		return (split);

	if (i >= n->len)
		split.hi = zero;
	else
	{
		split.hi.len = n->len - i;
		split.hi.num = &(n->num[i]);
	}

	if (i <= 0)
		split.lo = zero;
	else
	{
		split.lo.len = i < n->len ? i : n->len;
		split.lo.num = n->num;
	}

	return (split);
}

/*!
 * @brief get the maximum of 3 integers.
 *
 * @param a first.
 * @param b second.
 * @param c third.
 *
 * @return max value.
 */
static len_ty max_of_3(const len_ty a, const len_ty b, const len_ty c)
{
	len_ty maximum = a;

	if (b > maximum)
		maximum = b;

	if (c > maximum)
		maximum = c;

	return (maximum);
}

/* Why this number? Is testing the best way to determine the cutoff? */
/*! Minimum length of a bigint for Karatsuba algorithm. */
#define KARATSUBA_GAIN_CUTOFF 21

/*!
 * @brief multiply 2 `bigint`s using the Karatsuba algorithm.
 * @private @memberof bigint
 *
 * [Karatsuba_algorithm](https://en.wikipedia.org/wiki/Karatsuba_algorithm)
 *
 * Let ***x*** and ***y*** be represented as ***n***-digit strings in some
 * base ***B***. For any positive integer ***m*** less than ***n***, one can
 * write the two given numbers as:
 *
 * ```C
 * x = x1*B^m + x0
 * y = y1*B^m + y0
 * ```
 *
 * where ***x0*** and ***y0*** are less than ***B^m***. The product of the two
 * numbers is then:
 *
 * ```C
 * xy = (x1*B^m + x0)(y1*B^m + y0)
 *    = x1*y1*B^(2*m) + (x1*y0 + x0*y1)B^m + x0*y0
 *    = z2*B^(2*m) + z1*B^m + z0
 * ```
 *
 * Notice that ***z1*** can be rewritten as follows:
 *
 * ```C
 * z1 = x1*y0 + x0*y1
 *    = (x0 + x1)(y0 + y1) - x1y1 - x0y0
 *    = z3 - z2 - z0
 * ```
 *
 * thus reducing the number of multiplications in the formula by one.
 *
 * ```C
 * xy = z2*B^(2*m) + (z3 - z2 - z0)*B^m + z0
 * ```
 *
 * @param[in] n1 the first number.
 * @param[in] n2 the second number.
 *
 * @return pointer to the result, NULL on failure.
 */
static bigint *karatsuba_multiply(
	const bigint *const restrict n1, const bigint *const restrict n2
)
{
	if ((!n1 || !n2) || (n1->len < 1 || n2->len < 1) || (!n1->num || !n2->num))
		return (NULL);

	/**
	 * The Karatsuba algorithm only shows a significant speed up compared to
	 * the long multiplication algorithm if the number of "digits" in the
	 * `bigint`s is greater than `KARATSUBA_GAIN_CUTOFF`.
	 */
	if (n1->len < KARATSUBA_GAIN_CUTOFF || n2->len < KARATSUBA_GAIN_CUTOFF)
		return (_bi_trim(long_multiply(n1, n2)));

	const len_ty mid = (n1->len > n2->len ? n1->len / 2 : n2->len / 2);
	bigint *restrict result = NULL;
	bi_split x = bi_split_at(n1, mid);
	bi_split y = bi_split_at(n2, mid);
	bigint *const restrict z0 = karatsuba_multiply(&x.lo, &y.lo);
	bigint *const restrict z2 = karatsuba_multiply(&x.hi, &y.hi);
	bigint *const restrict xhi_plus_xlo = bi_add(&x.hi, &x.lo);
	bigint *const restrict yhi_plus_ylo = bi_add(&y.hi, &y.lo);
	bigint *restrict z3 = karatsuba_multiply(xhi_plus_xlo, yhi_plus_ylo);

	if (!z0 || !z2 || !z3)
		goto cleanup;

	// (z3 - z2 - z0) * BIGINT_BASE^mid
	bi_isubtract(z3, z2);
	bi_isubtract(z3, z0);
	z3 = _bi_resize(z3, z3->len + mid);
	if (!z3)
		goto cleanup;

	z3->len -= mid;
	bi_ishift_l(z3, mid);
	const len_ty z2_len = z2->len;

	// z2 * BIGINT_BASE^(2*mid)
	result = _bi_resize(z2, max_of_3(z2_len + mid * 2, z3->len, z0->len) + 1);
	if (!result)
		goto cleanup;

	result->len = z2_len;
	bi_ishift_l(result, mid * 2);

	// z2 * BIGINT_BASE^(2*mid) + (z3 - z2 - z0) * BIGINT_BASE^mid + z0
	bi_iadd(result, z3);
	bi_iadd(result, z0);
cleanup:
	_bi_free(xhi_plus_xlo);
	_bi_free(yhi_plus_ylo);
	_bi_free(z0);
	_bi_free(z3);
	return (result);
}

/*!
 * @brief handle multiplication of two signed `bigint`s.
 * @private @memberof bigint
 *
 * @param[in] n1 first number.
 * @param[in] n2 second number.
 *
 * @return pointer to the result, NULL on failure.
 */
static bigint *
multiply_negatives(bigint *const restrict n1, bigint *const restrict n2)
{
	const bool neg1 = n1->is_negative, neg2 = n2->is_negative;
	bigint *restrict result = NULL;

	n1->is_negative = false;
	n2->is_negative = false;
	if (neg1 && neg2)
	{
		/* -8 * -7 = 8*7 */
		result = karatsuba_multiply(n1, n2);
	}
	else if (neg1 || neg2)
	{
		/* -8 * 7 = -(8*7) */
		/* 8 * -7 = -(8*7) */
		result = karatsuba_multiply(n1, n2);
		if (result)
			result->is_negative = true;
	}

	n1->is_negative = neg1;
	n2->is_negative = neg2;
	return (result);
}

/*!
 * @brief handle multiplication of two `bigint`s.
 * @public @memberof bigint
 *
 * @param[in] n1 the first number.
 * @param[in] n2 the second number.
 *
 * @return pointer to result, NULL on failure.
 */
bigint *bi_multiply(bigint *const restrict n1, bigint *const restrict n2)
{
	if ((!n1 || !n2) || (n1->len < 0 || n2->len < 0))
		return (NULL);

	if (bi_isNaN(_bi_trim(n1)) || bi_isNaN(_bi_trim(n2)))
		return (_bi_alloc(0));

	bigint *restrict result = NULL;
	if (n1->is_negative || n2->is_negative)
		result = multiply_negatives(n1, n2);
	else
		result = karatsuba_multiply(n1, n2);

	return (_bi_trim(result));
}

/*!
 * @brief multiply a `bigint` with an int.
 * @public @memberof bigint
 *
 * @param[in] n1 pointer to the `bigint`.
 * @param[in] n2 the int to multiply with.
 *
 * @return pointer to the result.
 */
bigint *bi_multiply_int(bigint *const restrict n1, const intmax_t n2)
{
	if (!n1 || n1->len < 0)
		return (NULL);

	bigint num2 = {.len = 4, .is_negative = n2 < 0, .num = (digit_ty[6]){0}};

	return (bi_multiply(n1, int_to_bi(&num2, n2)));
}
