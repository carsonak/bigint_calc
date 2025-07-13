#include <stdbool.h> /* bool */

#include "test_Deque.h"
#include "xalloc.h"

#define MAX_STRING_LENGTH 256U

static char n1d[] = "one", n2d[] = "two", n3d[] = "three";

/**
 * @brief failing duplicating function.
 * @param dest unused.
 * @param d unused.
 *
 * @returns NULL.
 */
static bool fail_dup(char **const restrict dest, const char *const d)
{
	(void)dest;
	(void)d;
	return (false);
}

/**
 * @brief makes a copy of a string.
 * @param dest address to store the copied string.
 * @param s pointer to the string.
 *
 * @returns true on success, false on error.
 */
static bool dup_str(char **const restrict dest, const char *const s)
{
	if (!dest)
		return (false);

	if (!s)
	{
		*dest = NULL;
		return (true);
	}

	unsigned int len = 0;

	while (s[len] && len <= MAX_STRING_LENGTH)
		++len;

	*dest = xmalloc(sizeof(*s) * (len + 1));

	if (*dest)
	{
		memcpy(*dest, s, sizeof(*s) * len);
		(*dest)[len] = '\0';
	}

	return (true);
}

/**
 * @brief make a copy of an int.
 * @param dest address to store the copied int.
 * @param n the int.
 *
 * @returns true on success, false on failure.
 */
static bool
dup_llint(long long int *const restrict dest, const long long int n)
{
	if (!dest)
		return (false);

	*dest = n;
	return (true);
}

TEST(deque_creation, new_returns_empty_q)
{
	Deque_str *const dq = dq_str_new();

	REQUIRE(dq, "dq_str_new() returns non-null");
	CHECK(dq->len == 0);
	CHECK(dq->head == NULL);
	CHECK(dq->tail == NULL);
	dq_str_del(dq, NULL);
}

/* ###################################################################### */
/* ###################################################################### */

struct adding_items
{
	ListNode_str *n1, *n2, *n3;
	Deque_str *dq;
};

TEST_F_SETUP(adding_items)
{
	tau->dq = dq_str_new();
	REQUIRE(tau->dq, "dq_str_new() returns non-null");
}

TEST_F_TEARDOWN(adding_items)
{
	ln_str_del(tau->n1);
	ln_str_del(tau->n2);
	ln_str_del(tau->n3);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_str_del(tau->dq, NULL);
	memset(tau, 0, sizeof(*tau));
}

TEST(adding_items, pushtail_null_q_returns_null)
{
	CHECK(dq_str_push_tail(NULL, NULL, NULL) == NULL);
	CHECK(dq_str_push_tail(NULL, n1d, NULL) == NULL);
	CHECK(dq_str_push_tail(NULL, NULL, dup_str) == NULL);
	CHECK(dq_str_push_tail(NULL, n1d, dup_str) == NULL);
}

TEST(adding_items, pushhead_null_q_returns_null)
{
	CHECK(dq_str_push_head(NULL, NULL, NULL) == NULL);
	CHECK(dq_str_push_head(NULL, n1d, NULL) == NULL);
	CHECK(dq_str_push_head(NULL, NULL, dup_str) == NULL);
	CHECK(dq_str_push_head(NULL, n1d, dup_str) == NULL);
}

TEST_F(adding_items, pushtail_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = dq_str_push_tail(tau->dq, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK(ln_str_get_data(tau->dq->head) == NULL);
	CHECK(ln_str_get_data(tau->dq->tail) == NULL);
}

TEST_F(adding_items, pushhead_q_null_null_adds_node_with_NULL_data)
{
	tau->n1 = dq_str_push_head(tau->dq, NULL, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK(ln_str_get_data(tau->dq->head) == NULL);
	CHECK(ln_str_get_data(tau->dq->tail) == NULL);
}

TEST_F(adding_items, pushtail_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_STREQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_STREQ(ln_str_get_data(tau->dq->tail), n1d);

	xfree(ln_str_set_data(tau->dq->head, NULL));
}

TEST_F(adding_items, pushhead_q_d_f_adds_node_with_duplicated_data)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, dup_str);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_STREQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_STREQ(ln_str_get_data(tau->dq->tail), n1d);

	xfree(ln_str_set_data(tau->dq->head, NULL));
}

TEST_F(adding_items, pushtail_3_nodes)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);

	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);

	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushhead_3_nodes)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);

	tau->n2 = dq_str_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);

	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushtail_q_d_failfunc_returns_NULL)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "push_tail() should return NULL on failure");
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(adding_items, pushhead_q_d_failfunc_returns_NULL)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, fail_dup);

	CHECK(tau->n1 == NULL, "push_head() should return NULL on failure");
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(adding_items, pushtail_2_nodes_fail_on_2nd)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_str_push_tail(tau->dq, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "push_tail() should return NULL on failure");
	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushhead_2_nodes_fail_on_2nd)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_str_push_head(tau->dq, n2d, fail_dup);

	CHECK(tau->n2 == NULL, "push_head() should return NULL on failure");
	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushhead_pushtail_pushtail)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);

	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushhead_pushtail_pushhead)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(adding_items, pushhead_pushhead_pushtail)
{
	tau->n1 = dq_str_push_head(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_head() should return non-null pointer");
	tau->n2 = dq_str_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushtail_pushhead_pushtail)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_str_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);

	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(adding_items, pushtail_pushhead_pushhead)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_str_push_head(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_head() should return non-null pointer");
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(adding_items, pushtail_pushtail_pushhead)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_head() should return non-null pointer");

	CHECK(tau->dq->len == 3);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

/* ###################################################################### */
/* ########################## removing_items ############################ */
/* ###################################################################### */

struct removing_items
{
	ListNode_str *n1, *n2, *n3;
	Deque_str *dq;
};

TEST_F_SETUP(removing_items)
{
	tau->dq = dq_str_new();
	REQUIRE(tau->dq, "dq_str_new() returns non-null");
}

TEST_F_TEARDOWN(removing_items)
{
	ln_str_del(tau->n1);
	ln_str_del(tau->n2);
	ln_str_del(tau->n3);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_str_del(tau->dq, NULL);
	memset(tau, 0, sizeof(*tau));
}

TEST(removing_items, poptail_null_should_return_NULL)
{
	CHECK(dq_str_pop_tail(NULL) == NULL);
}

TEST_F(removing_items, poptail_empty_q_changes_nothing)
{
	CHECK(dq_str_pop_tail(tau->dq) == NULL);

	CHECK(tau->dq->len == 0);
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(removing_items, poptail_1_node_from_1)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ(dq_str_pop_tail(tau->dq), n1d);
	tau->n1 = NULL;
	CHECK(tau->dq->len == 0);
	CHECK_PTR_EQ(tau->dq->head, NULL);
	CHECK_PTR_EQ(tau->dq->tail, NULL);
}

TEST_F(removing_items, poptail_1_node_from_2)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ(dq_str_pop_tail(tau->dq), n2d);
	tau->n2 = NULL;
	CHECK(tau->dq->len == 1);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(removing_items, poptail_1_node_from_3)
{
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	REQUIRE(tau->n1 != NULL, "push_tail() should return non-null pointer");
	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	REQUIRE(tau->n2 != NULL, "push_tail() should return non-null pointer");
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3 != NULL, "push_tail() should return non-null pointer");

	CHECK_STREQ(dq_str_pop_tail(tau->dq), n3d);
	tau->n3 = NULL;
	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

/* ###################################################################### */
/* ##################### add_and_remove_pushtail ######################### */
/* ###################################################################### */

struct add_and_remove_pushtail
{
	ListNode_str *n1, *n2, *n3, *n4;
	Deque_str *dq;
};

TEST_F_SETUP(add_and_remove_pushtail)
{
	tau->dq = dq_str_new();
	tau->n1 = dq_str_push_tail(tau->dq, n1d, NULL);
	tau->n2 = dq_str_push_tail(tau->dq, n2d, NULL);
	if (!tau->dq || !tau->n1 || !tau->n2)
	{
		ln_str_del(tau->n1);
		ln_str_del(tau->n2);
		tau->dq->head = NULL;
		tau->dq->tail = NULL;
		tau->dq = dq_str_del(tau->dq, NULL);
	}

	REQUIRE(tau->dq, "dq_str_new() returns non-null");
	REQUIRE(tau->n1, "push_tail() returns non-null");
	REQUIRE(tau->n2, "push_tail() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove_pushtail)
{
	ln_str_del(tau->n1);
	ln_str_del(tau->n2);
	ln_str_del(tau->n3);
	ln_str_del(tau->n4);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_str_del(tau->dq, NULL);
	memset(tau, 0, sizeof(*tau));
}

TEST_F(add_and_remove_pushtail, pushtail_poptail)
{
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_tail(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushtail, pushtail_pophead)
{
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushtail, pushhead_pophead)
{
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_head(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushtail, pushhead_poptail)
{
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushtail, poptail_pushtail)
{
	CHECK(dq_str_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushtail, poptail_pushhead)
{
	CHECK(dq_str_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushtail, pophead_pushhead)
{
	CHECK(dq_str_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushtail, pophead_pushtail)
{
	CHECK(dq_str_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

/* ###################################################################### */
/* ###################### add_and_remove_pushhead ######################## */
/* ###################################################################### */

struct add_and_remove_pushhead
{
	ListNode_str *n1, *n2, *n3, *n4;
	Deque_str *dq;
};

TEST_F_SETUP(add_and_remove_pushhead)
{
	tau->dq = dq_str_new();
	tau->n2 = dq_str_push_head(tau->dq, n2d, NULL);
	tau->n1 = dq_str_push_head(tau->dq, n1d, NULL);
	if (!tau->dq || !tau->n1 || !tau->n2)
	{
		ln_str_del(tau->n1);
		ln_str_del(tau->n2);
		tau->dq->head = NULL;
		tau->dq->tail = NULL;
		tau->dq = dq_str_del(tau->dq, NULL);
	}

	REQUIRE(tau->dq, "dq_str_new() returns non-null");
	REQUIRE(tau->n2, "push_head() returns non-null");
	REQUIRE(tau->n1, "push_head() returns non-null");
}

TEST_F_TEARDOWN(add_and_remove_pushhead)
{
	ln_str_del(tau->n1);
	ln_str_del(tau->n2);
	ln_str_del(tau->n3);
	ln_str_del(tau->n4);
	tau->dq->head = NULL;
	tau->dq->tail = NULL;
	tau->dq = dq_str_del(tau->dq, NULL);
	memset(tau, 0, sizeof(*tau));
}

TEST_F(add_and_remove_pushhead, pushtail_poptail)
{
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_tail(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushhead, pushtail_pophead)
{
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushhead, pushhead_pophead)
{
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_head(tau->dq) == n3d);
	tau->n3 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushhead, pushhead_poptail)
{
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);
	CHECK(dq_str_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushhead, poptail_pushtail)
{
	CHECK(dq_str_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n1d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

TEST_F(add_and_remove_pushhead, poptail_pushhead)
{
	CHECK(dq_str_pop_tail(tau->dq) == n2d);
	tau->n2 = NULL;
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n1d);
}

TEST_F(add_and_remove_pushhead, pophead_pushhead)
{
	CHECK(dq_str_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_str_push_head(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n3d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n2d);
}

TEST_F(add_and_remove_pushhead, pophead_pushtail)
{
	CHECK(dq_str_pop_head(tau->dq) == n1d);
	tau->n1 = NULL;
	tau->n3 = dq_str_push_tail(tau->dq, n3d, NULL);
	REQUIRE(tau->n3);

	CHECK(tau->dq->len == 2);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->head), n2d);
	CHECK_PTR_EQ(ln_str_get_data(tau->dq->tail), n3d);
}

/* ###################################################################### */
/* ######################### deleting_deque ############################# */
/* ###################################################################### */

static void free_str(char *p) { xfree(p); }

TEST(deleting_deque, delete_should_clear_all_items)
{
	Deque_str *dq = dq_str_new();
	ListNode_str *n1 = dq_str_push_tail(dq, n1d, NULL);
	ListNode_str *n2 = dq_str_push_tail(dq, n2d, NULL);
	ListNode_str *n3 = dq_str_push_tail(dq, n3d, NULL);

	if (!dq || !n1 || !n2 || !n3)
	{
		ln_str_del(n1);
		n1 = NULL;
		ln_str_del(n2);
		n2 = NULL;
		ln_str_del(n3);
		n3 = NULL;
		dq->head = NULL;
		dq->tail = NULL;
		dq = dq_str_del(dq, NULL);
	}

	REQUIRE((dq && n1 && n2 && n3));

	dq = dq_str_del(dq, NULL);
}

TEST(deleting_deque, delete_f_should_clear_all_items)
{
	Deque_str *dq = dq_str_new();
	ListNode_str *n1 = dq_str_push_tail(dq, n1d, dup_str);
	ListNode_str *n2 = dq_str_push_tail(dq, n2d, dup_str);
	ListNode_str *n3 = dq_str_push_tail(dq, n3d, dup_str);

	if (!dq || !n1 || !n2 || !n3)
	{
		xfree(ln_str_del(n1));
		n1 = NULL;
		xfree(ln_str_del(n2));
		n2 = NULL;
		xfree(ln_str_del(n3));
		n3 = NULL;
		dq->head = NULL;
		dq->tail = NULL;
		dq = dq_str_del(dq, NULL);
	}

	REQUIRE((dq && n1 && n2 && n3));

	dq = dq_str_del(dq, free_str);
}

/* ###################################################################### */
/* ############################## dqfa ################################## */
/* ###################################################################### */

TEST(dqfa, dqfa_invalid_args)
{
	long long int arr[] = {1, 2, 3, 4, 5};
	const size_t arr_len = (sizeof(arr) / sizeof(*arr));

	CHECK(
		dq_llint_from_array(NULL, 0, NULL, NULL) == NULL,
		"data_array and len are required"
	);

	CHECK(
		dq_llint_from_array(NULL, arr_len, NULL, NULL) == NULL,
		"data_array is required"
	);

	CHECK(dq_llint_from_array(arr, 0, NULL, NULL) == NULL, "len is required");

	CHECK(
		dq_llint_from_array(arr, arr_len, dup_llint, NULL) == NULL,
		"a dup function should always be accompanied by a delete function"
	);
}

TEST(dqfa, deque_from_array)
{
	long long int arr[] = {1, 2, 3, 4, 5};
	const size_t arr_len = (sizeof(arr) / sizeof(*arr));
	Deque_llint *dq = dq_llint_from_array(arr, arr_len, NULL, NULL);

	REQUIRE(dq, "dq_llint_from_array() should return non-null pointer");

	CHECK(
		dq->len == arr_len, "there should be %zu items in the Deque_str",
		arr_len
	);
	CHECK(dq_llint_pop_tail(dq) == arr[4]);
	CHECK(dq_llint_pop_tail(dq) == arr[3]);
	CHECK(dq_llint_pop_tail(dq) == arr[2]);
	CHECK(dq_llint_pop_tail(dq) == arr[1]);
	CHECK(dq_llint_pop_tail(dq) == arr[0]);
	CHECK(dq->len == 0);
	CHECK(dq->head == NULL);
	CHECK(dq->tail == NULL);

	dq = dq_llint_del(dq, NULL);
}
