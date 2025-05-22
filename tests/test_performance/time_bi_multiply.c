#include "_bi_internals.h"
#include "bigint.h"
#include <errno.h>
#include <inttypes.h>
#include <printf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static bool str_to_bignum(const char *arr_str, bigint *const restrict n)
{
	const char *restrict len_keyword = "len=";
	char *num_ptr = NULL;

	if (!arr_str || !n)
		return (false);

	for (; *len_keyword != '\0'; ++arr_str, ++len_keyword)
	{
		if (*arr_str != *len_keyword)
			return (false);
	}

	n->len = strtoimax(arr_str, &num_ptr, 10);
	if (*num_ptr != ' ')
		return (false);

	num_ptr += 2;
	n->num = malloc(sizeof(*n->num) * n->len);
	if (!n->num)
		return (false);

	for (len_type i = 0; i < n->len; i++)
	{
		arr_str = num_ptr;
		n->num[i] = strtoimax(arr_str, &num_ptr, 10);
		if (*num_ptr != ',' && *num_ptr != '}')
		{
			free(n->num);
			n->num = NULL;
			return (false);
		}

		num_ptr += 2;
	}

	return (true);
}

static bool
time_bi_multiply(bigint *restrict const n1, bigint *restrict const n2)
{
	bool passed = true;

	printf(": %5" PRIdMAX "x%zub", n1->len, sizeof(*n1->num));

	const clock_t start = clock();

	bigint *const output = bi_multiply(n1, n2);

	const clock_t stop = clock();
	const double time_taken = (double)(stop - start) / CLOCKS_PER_SEC;

	if (!output)
	{
		printf(" : FAILED");
		passed = false;
	}

	bi_delete(output);
	printf(" - %fs\n", time_taken);
	return (passed);
}

int main(void)
{
	char *line1 = NULL, *line2 = NULL;
	bool ok = true;
	size_t line1_sz = 0, line2_sz = 0, i = 1;
	FILE *file = fopen("tests_data/speed_test_nums_as_arrays.txt", "r");

	if (!file)
	{
		perror("Error opening file");
		return (1);
	}

#ifdef LONG_MULTIPLY
	printf("Long Multiplication\n");
#else
	printf("Karatsuba Multiplication\n");
#endif
	errno = 0;
	while (ok && getline(&line1, &line1_sz, file) > 0 &&
		   getline(&line2, &line2_sz, file) > 0)
	{
		bigint n1 = {0}, n2 = {0};

		errno = 0;
		if (!str_to_bignum(line1, &n1) || !str_to_bignum(line2, &n2))
			ok = false;

		free(line1);
		line1 = NULL;
		free(line2);
		line2 = NULL;
		if (ok)
		{
			printf("%.2zu", i++);
			if (!time_bi_multiply(&n1, &n2))
				ok = false;
		}

		free(n1.num);
		n1.num = NULL;
		free(n2.num);
		n2.num = NULL;
	}

	if (errno)
		perror("Error reading file");

	free(line1);
	free(line2);
	fclose(file);
	return (!ok);
}
