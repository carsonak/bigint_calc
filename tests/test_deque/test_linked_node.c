#include "test_deque.h"

#define MAX_STRING_LENGTH ((unsigned int)256)

static char original[] = "original";
static char n1d[] = "one", n2d[] = "two", n3d[] = "three";

/**
 * fail_dup - failing duplicating function.
 * @d: unused.
 *
 * Return: NULL.
 */
static void *fail_dup(void const *const d)
{
	(void)d;
	return (NULL);
}

/**
 * dup_str - makes a copy of a string.
 * @str: pointer to the string.
 *
 * Return: pointer to the new string, NULL on failure.
 */
static void *dup_str(void const *const str)
{
	char const *const s = str;

	if (!s)
		return (NULL);

	unsigned int len = 0;

	while (s[len] && len <= MAX_STRING_LENGTH)
		++len;

	char *const s_dup = malloc(sizeof(*s_dup) * (len + 1));

	if (s_dup)
	{
		memcpy(s_dup, s, sizeof(*s_dup) * len);
		s_dup[len] = '\0';
	}

	return (s_dup);
}

/* ###################################################################### */
/* ########################## node_creation ############################# */
/* ###################################################################### */

struct node_creation
{
	linked_node *output;
};

TEST_F_SETUP(node_creation) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(node_creation) { tau->output = node_del(tau->output); }

TEST_F(node_creation, new_NULL_NULL_returns_NULL)
{
	tau->output = node_new(NULL, NULL);

	REQUIRE_PTR_NE(tau->output, NULL);

	CHECK_PTR_EQ(node_get_data(tau->output), NULL);
	CHECK_PTR_EQ(node_get_next(tau->output), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->output), NULL);
}

TEST_F(node_creation, new_d_NULL_returns_node_with_unchanged_d_pointer)
{
	char input[] = "input";
	tau->output = node_new(input, NULL);

	REQUIRE_PTR_NE(tau->output, NULL);

	CHECK_PTR_EQ(node_get_data(tau->output), input);
	CHECK_PTR_EQ(node_get_next(tau->output), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->output), NULL);
}

TEST_F(node_creation, new_NULL_f_returns_NULL)
{
	tau->output = node_new(NULL, dup_str);

	REQUIRE_PTR_NE(tau->output, NULL);

	CHECK_PTR_EQ(node_get_data(tau->output), NULL);
	CHECK_PTR_EQ(node_get_next(tau->output), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->output), NULL);
}

TEST_F(node_creation, new_d_f_returns_correct_node)
{
	char input[] = "input";
	tau->output = node_new(input, dup_str);

	REQUIRE_PTR_NE(tau->output, NULL);
	char *const output_data = node_get_data(tau->output);

	CHECK_PTR_NE(output_data, input);
	CHECK_STREQ(output_data, input);
	CHECK_PTR_EQ(node_get_next(tau->output), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->output), NULL);
	free(output_data);
}

TEST(node_creation, new_d_faildup_returns_NULL)
{
	char input[] = "input";

	REQUIRE_PTR_EQ(node_new(input, fail_dup), NULL);
}

/* ###################################################################### */
/* ########################## data_get ################################## */
/* ###################################################################### */

struct data_get
{
	linked_node *output;
};

TEST_F_SETUP(data_get) { memset(tau, 0, sizeof(*tau)); }

TEST_F_TEARDOWN(data_get) { tau->output = node_del(tau->output); }

TEST_F(data_get, get_data_returns_correct_data)
{
	char input[] = "input";
	tau->output = node_new(input, NULL);

	REQUIRE_PTR_NE(tau->output, NULL);
	CHECK_PTR_EQ(node_get_data(tau->output), input);
}

TEST_F(data_get, get_data_returns_correct_duplicated_data)
{
	char input[] = "input";
	tau->output = node_new(input, dup_str);

	REQUIRE_PTR_NE(tau->output, NULL);
	char *const curr_data = node_get_data(tau->output);

	CHECK(curr_data != input);
	CHECK_STREQ(curr_data, input);
	free(curr_data);
}

/* ###################################################################### */
/* ############################# data_swap ############################## */
/* ###################################################################### */

struct data_swap
{
	linked_node n1, n2;
};

TEST_F_SETUP(data_swap)
{
	node_set_data(&tau->n1, n1d);
	node_set_data(&tau->n2, n2d);
}

TEST_F_TEARDOWN(data_swap) { (void)tau; }

TEST_F(data_swap, swap_NULLs)
{
	CHECK_PTR_EQ(node_swap(NULL, NULL), NULL);
	CHECK_PTR_EQ(node_swap(&tau->n1, NULL), NULL);
	CHECK_PTR_EQ(node_swap(NULL, &tau->n1), &tau->n1);
}

TEST_F(data_swap, swap_n_d_NULL)
{
	CHECK_PTR_EQ(node_get_data(node_swap(&tau->n1, &tau->n2)), n1d);
	CHECK_PTR_EQ(node_get_data(&tau->n1), n2d);
}

/* ###################################################################### */
/* ###################################################################### */

struct node_insertion
{
	linked_node *n1, *n2, *n3;
};

TEST_F_SETUP(node_insertion)
{
	tau->n1 = node_new(n1d, NULL);
	tau->n2 = node_new(n2d, NULL);
	tau->n3 = node_new(n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		tau->n1 = node_del(tau->n1);
		tau->n2 = node_del(tau->n2);
		tau->n3 = node_del(tau->n3);
	}

	REQUIRE(tau->n1 != NULL);
	REQUIRE(tau->n2 != NULL);
	REQUIRE(tau->n3 != NULL);
}

TEST_F_TEARDOWN(node_insertion)
{
	tau->n1 = node_del(tau->n1);
	tau->n2 = node_del(tau->n2);
	tau->n3 = node_del(tau->n3);
}

TEST(node_insertion, insert_NULLs)
{
	CHECK_PTR_EQ(node_insert_after(NULL, NULL), NULL);
	CHECK(node_insert_before(NULL, NULL) == NULL);
}

TEST_F(node_insertion, insert_after_NULL)
{
	REQUIRE_PTR_EQ(node_insert_after(NULL, tau->n2), tau->n2);

	CHECK_PTR_EQ(node_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_NULL_after)
{
	REQUIRE_PTR_EQ(node_insert_after(tau->n1, NULL), NULL);

	CHECK_PTR_EQ(node_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
}

TEST_F(node_insertion, insert_after_1_node)
{
	REQUIRE_PTR_EQ(node_insert_after(tau->n1, tau->n2), tau->n2);

	CHECK_PTR_EQ(node_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_next(tau->n1), tau->n2);

	CHECK_PTR_EQ(node_get_prev(tau->n2), tau->n1);
	CHECK_PTR_EQ(node_get_prev(tau->n1), NULL);

	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_after_2_nodes)
{
	linked_node *output = node_insert_after(tau->n1, tau->n2);

	REQUIRE(output == tau->n2);
	output = node_insert_after(tau->n2, tau->n3);
	REQUIRE(output == tau->n3);

	CHECK_PTR_EQ(node_get_next(tau->n3), NULL);
	CHECK_PTR_EQ(node_get_next(tau->n2), tau->n3);
	CHECK_PTR_EQ(node_get_next(tau->n1), tau->n2);

	CHECK_PTR_EQ(node_get_prev(tau->n3), tau->n2);
	CHECK_PTR_EQ(node_get_prev(tau->n2), tau->n1);
	CHECK_PTR_EQ(node_get_prev(tau->n1), NULL);

	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(node_get_data(tau->n3), n3d);
}

TEST_F(node_insertion, insert_after_between_2_nodes)
{
	linked_node *output = node_insert_after(tau->n1, tau->n3);

	REQUIRE(output == tau->n3);
	output = node_insert_after(tau->n1, tau->n2);
	REQUIRE(output == tau->n2);

	CHECK_PTR_EQ(node_get_next(tau->n3), NULL);
	CHECK_PTR_EQ(node_get_next(tau->n2), tau->n3);
	CHECK_PTR_EQ(node_get_next(tau->n1), tau->n2);

	CHECK_PTR_EQ(node_get_prev(tau->n3), tau->n2);
	CHECK_PTR_EQ(node_get_prev(tau->n2), tau->n1);
	CHECK_PTR_EQ(node_get_prev(tau->n1), NULL);

	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(node_get_data(tau->n3), n3d);
}

TEST_F(node_insertion, insert_before_NULL)
{
	REQUIRE(node_insert_before(NULL, tau->n2) == tau->n2);

	CHECK_PTR_EQ(node_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_NULL_before)
{
	REQUIRE_PTR_EQ(node_insert_before(tau->n1, NULL), NULL);

	CHECK_PTR_EQ(node_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
}

TEST_F(node_insertion, insert_before_1_node)
{
	REQUIRE(node_insert_before(tau->n1, tau->n2) == tau->n2);

	CHECK_PTR_EQ(node_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_next(tau->n2), tau->n1);

	CHECK_PTR_EQ(node_get_prev(tau->n1), tau->n2);
	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);

	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
}

TEST_F(node_insertion, insert_before_2_nodes)
{
	linked_node *output = node_insert_before(tau->n1, tau->n2);

	REQUIRE(output == tau->n2);
	output = node_insert_before(tau->n2, tau->n3);
	REQUIRE(output == tau->n3);

	CHECK_PTR_EQ(node_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_next(tau->n2), tau->n1);
	CHECK_PTR_EQ(node_get_next(tau->n3), tau->n2);

	CHECK_PTR_EQ(node_get_prev(tau->n1), tau->n2);
	CHECK_PTR_EQ(node_get_prev(tau->n2), tau->n3);
	CHECK_PTR_EQ(node_get_prev(tau->n3), NULL);

	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(node_get_data(tau->n3), n3d);
}

TEST_F(node_insertion, insert_before_between_2_nodes)
{
	linked_node *output = node_insert_before(tau->n1, tau->n3);

	REQUIRE(output == tau->n3);
	output = node_insert_before(tau->n1, tau->n2);
	REQUIRE(output == tau->n2);

	CHECK_PTR_EQ(node_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_next(tau->n2), tau->n1);
	CHECK_PTR_EQ(node_get_next(tau->n3), tau->n2);

	CHECK_PTR_EQ(node_get_prev(tau->n1), tau->n2);
	CHECK_PTR_EQ(node_get_prev(tau->n2), tau->n3);
	CHECK_PTR_EQ(node_get_prev(tau->n3), NULL);

	CHECK_PTR_EQ(node_get_data(tau->n1), n1d);
	CHECK_PTR_EQ(node_get_data(tau->n2), n2d);
	CHECK_PTR_EQ(node_get_data(tau->n3), n3d);
}

/* ###################################################################### */
/* ########################### node_deletion ############################ */
/* ###################################################################### */

struct node_deletion
{
	linked_node *n1, *n2, *n3;
};

TEST_F_SETUP(node_deletion)
{
	tau->n1 = node_new(n1d, NULL);
	tau->n2 = node_new(n2d, NULL);
	tau->n3 = node_new(n3d, NULL);
	if (!tau->n1 || !tau->n2 || !tau->n3)
	{
		tau->n1 = node_del(tau->n1);
		tau->n2 = node_del(tau->n2);
		tau->n3 = node_del(tau->n3);
	}

	REQUIRE(tau->n1 != NULL);
	REQUIRE(tau->n2 != NULL);
	REQUIRE(tau->n3 != NULL);

	/* n1->n2->n3 */
	node_insert_after(node_insert_after(tau->n1, tau->n2), tau->n3);
}

TEST_F_TEARDOWN(node_deletion)
{
	tau->n1 = node_del(tau->n1);
	tau->n2 = node_del(tau->n2);
	tau->n3 = node_del(tau->n3);
}

TEST(node_deletion, remove_NULL) { CHECK_PTR_EQ(node_del(NULL), NULL); }

TEST(node_deletion, remove_n)
{
	linked_node *n1 = node_new(original, NULL);
	char *out = node_del(n1) ;

	CHECK_PTR_EQ(out, original);
}

TEST_F(node_deletion, remove_first_node_in_list)
{
	CHECK_PTR_EQ(node_del(tau->n1), n1d);
	tau->n1 = NULL;

	CHECK_PTR_EQ(node_get_next(tau->n2), tau->n3);
	CHECK_PTR_EQ(node_get_next(tau->n3), NULL);

	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n3), tau->n2);
}

TEST_F(node_deletion, remove_middle_node_in_list)
{
	CHECK_PTR_EQ(node_del(tau->n2), n2d);
	tau->n2 = NULL;

	CHECK_PTR_EQ(node_get_next(tau->n1), tau->n3);
	CHECK_PTR_EQ(node_get_next(tau->n3), NULL);

	CHECK_PTR_EQ(node_get_prev(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n3), tau->n1);
}

TEST_F(node_deletion, remove_last_node_in_list)
{
	CHECK_PTR_EQ(node_del(tau->n3), n3d);
	tau->n3 = NULL;

	CHECK_PTR_EQ(node_get_next(tau->n1), tau->n2);
	CHECK_PTR_EQ(node_get_next(tau->n2), NULL);

	CHECK_PTR_EQ(node_get_prev(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n2), tau->n1);
}

TEST_F(node_deletion, remove_first_node_of_2_in_list)
{
	node_del(tau->n3);
	tau->n3 = NULL;

	CHECK_PTR_EQ(node_del(tau->n1), n1d);
	tau->n1 = NULL;

	CHECK_PTR_EQ(node_get_next(tau->n2), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n2), NULL);
}

TEST_F(node_deletion, remove_second_node_of_2_in_list)
{
	node_del(tau->n3);
	tau->n3 = NULL;

	CHECK_PTR_EQ(node_del(tau->n2), n2d);
	tau->n2 = NULL;

	CHECK_PTR_EQ(node_get_next(tau->n1), NULL);
	CHECK_PTR_EQ(node_get_prev(tau->n1), NULL);
}

TEST(node_deletion, clear_NULL)
{
	/* Should produce no leaks or UB. */
	linked_list_del(NULL, NULL);
}

TEST_F(node_deletion, clear_ll)
{
	/* Should produce no leaks or UB. */
	linked_list_del(tau->n1, NULL);
	tau->n1 = NULL;
	tau->n2 = NULL;
	tau->n3 = NULL;
}
