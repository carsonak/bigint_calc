#include "tests.h"

TEST(numstr_alloc, alloc_zero_len_numstr)
{
	numstr *ns = _numstr_alloc(0);

	REQUIRE(ns, "alloc should return non-null");
	CHECK(
		ns->len == 0, "character array length should be %" PRIdMAX, (len_type)0
	);
	CHECK(ns->str == NULL, "character array should be NULL");

	_numstr_free(ns);
}

TEST(numstr_alloc, alloc_sized_numstr)
{
	numstr *ns = _numstr_alloc(10);

	REQUIRE(ns, "alloc should return non-null");
	CHECK(
		ns->len == 10, "character array length should be %" PRIdMAX,
		(len_type)10
	);
	CHECK(ns->str, "character array should be non-null");
	ns->str[10 - 1] = 'A';  // memory checkers should not complain

	_numstr_free(ns);
}
