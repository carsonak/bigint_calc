#include "test_number_parsing.h"

TEST(numstr_alloc, alloc_zero_len_numstr)
{
	const len_ty len = 0;
	numstr *ns = _numstr_alloc(len);

	REQUIRE_PTR_NE(ns, NULL);
	CHECK(ns->len == len);
	CHECK_PTR_EQ(ns->str, NULL);

	_numstr_free(ns);
}

TEST(numstr_alloc, alloc_sized_numstr)
{
	const len_ty len = 10;
	numstr *ns = _numstr_alloc(len);

	REQUIRE_PTR_NE(ns, NULL);
	CHECK(ns->len == len);
	CHECK_PTR_NE(ns->str, NULL);
	ns->str[len - 1] = 'A';  // memory checkers should not complain

	_numstr_free(ns);
}

TEST(numstr_free, null_pointer) { CHECK_PTR_EQ(_numstr_free(NULL), NULL); }
