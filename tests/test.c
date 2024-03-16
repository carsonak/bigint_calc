#include <criterion/parameterized.h>
#include <criterion/new/assert.h>
#include <stdio.h>

/* Basic usage */

ParameterizedTestParameters(params, simple)
{
	static int vals[] = {1, 2, 3};

	return (cr_make_param_array(int, vals, sizeof(vals) / sizeof(int)));
}

ParameterizedTest(int *val, params, simple)
{
	cr_fatal("Parameter: %d", *val);
}
