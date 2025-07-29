#ifndef DEQUE_UNIQUE_SUFFIX
	#error "Missing definition for `DEQUE_UNIQUE_SUFFIX`."
#endif

#ifndef DEQUE_DATA_TYPE
	#error "Missing definition for `DEQUE_DATA_TYPE`."
#endif

#include <string.h> /* strcpy */

#define LISTNODE_UNIQUE_SUFFIX DEQUE_UNIQUE_SUFFIX
#define LISTNODE_DATA_TYPE DEQUE_DATA_TYPE
#include "ListNode_methods.c"

#define DQ_CONCAT_PLAIN(tok0, tok1) tok0##tok1
#define DQ_CONCAT(tok0, tok1) DQ_CONCAT_PLAIN(tok0, tok1)

#define DEQUE_STRUCT_TAG DQ_CONCAT(Deque_, DEQUE_UNIQUE_SUFFIX)

#define DEQUE_METHODNAME(name)                                                \
	DQ_CONCAT(DQ_CONCAT(dq_, DEQUE_UNIQUE_SUFFIX), DQ_CONCAT(_, name))

#define LISTNODE_STRUCT_TAG DQ_CONCAT(ListNode_, DEQUE_UNIQUE_SUFFIX)

#define LISTNODE_METHODNAME(name)                                             \
	DQ_CONCAT(DQ_CONCAT(ln_, DEQUE_UNIQUE_SUFFIX), DQ_CONCAT(_, name))

#define LIST_METHODNAME(name)                                                 \
	DQ_CONCAT(DQ_CONCAT(list_, DEQUE_UNIQUE_SUFFIX), DQ_CONCAT(_, name))

/**
 * @brief allocate and initialise memory for a deque.
 *
 * @returns pointer to the deque, NULL on failure.
 */
DEQUE_STRUCT_TAG *DEQUE_METHODNAME(new)(void)
{
	return (xcalloc(1, sizeof(DEQUE_STRUCT_TAG)));
}

/**
 * @brief free all the nodes of a deque.
 * @param dq the deque to operate on.
 * @param data_free pointer to a function that can free the data in the nodes.
 */
void DEQUE_METHODNAME(clear)(
	DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * data_free
)
{
	if (!dq || !dq->head)
		return;

	LISTNODE_STRUCT_TAG *next_node = LISTNODE_METHODNAME(get_next)(dq->head);

	while (dq->head)
	{
		DEQUE_DATA_TYPE d = LISTNODE_METHODNAME(del)(dq->head);

		if (data_free)
			data_free(d);

		dq->head = next_node;
		next_node = dq->head ? LISTNODE_METHODNAME(get_next)(dq->head) : NULL;
	}

	dq->head = NULL;
	dq->tail = NULL;
	dq->len = 0;
}

/**
 * @brief free memory of a deque.
 * @param dq pointer to the deque to free.
 * @param data_free pointer to a function that can free data in the nodes.
 *
 * @returns NULL always.
 */
void *DEQUE_METHODNAME(del)(
	DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * data_free
)
{
	DEQUE_METHODNAME(clear)(dq, data_free);
	xfree(dq);
	return (NULL);
}

/**
 * @brief add a new node to the head of the deque.
 * @param dq the deque to operate on.
 * @param data data that the new node will hold.
 * @param data_dup function that can duplicate `data`.
 *
 * @returns pointer to the new node, NULL on failure.
 */
LISTNODE_STRUCT_TAG *DEQUE_METHODNAME(push_head)(
	DEQUE_STRUCT_TAG *const restrict dq, DEQUE_DATA_TYPE data,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * data_dup
)
{
	if (!dq)
		return (NULL);

	LISTNODE_STRUCT_TAG *const restrict nw =
		LISTNODE_METHODNAME(new)(data, data_dup);

	if (!nw)
		return (NULL);

	dq->head = LISTNODE_METHODNAME(insert_before)(dq->head, nw);
	if (!dq->tail)
		dq->tail = nw;

	++(dq->len);
	return (nw);
}

/**
 * @brief add a node to the tail of a deque.
 * @param dq the deque to operate on.
 * @param data data that the node will hold.
 * @param data_dup function that can duplicate data,
 * if NULL a simple assignment of the data is done.
 *
 * @returns pointer to the newly added node, NULL on error.
 */
LISTNODE_STRUCT_TAG *DEQUE_METHODNAME(push_tail)(
	DEQUE_STRUCT_TAG *const restrict dq, DEQUE_DATA_TYPE data,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * data_dup
)
{
	if (!dq)
		return (NULL);

	LISTNODE_STRUCT_TAG *const restrict nw =
		LISTNODE_METHODNAME(new)(data, data_dup);
	if (!nw)
		return (NULL);

	dq->tail = LISTNODE_METHODNAME(insert_after)(dq->tail, nw);
	if (!dq->head)
		dq->head = nw;

	++(dq->len);
	return (nw);
}

/**
 * @brief pop a node from the head of a deque.
 * @param dq the deque to operate on.
 *
 * @returns the data of the popped node, 0 if dq or head is NULL.
 */
DEQUE_DATA_TYPE DEQUE_METHODNAME(pop_head)(DEQUE_STRUCT_TAG *const restrict dq)
{
	DEQUE_DATA_TYPE zero;

	memset(&zero, 0, sizeof(zero));
	if (!dq || !dq->head)
		return (zero);

	LISTNODE_STRUCT_TAG *const node = dq->head;

	dq->head = LISTNODE_METHODNAME(get_next)(node);
	DEQUE_DATA_TYPE d = LISTNODE_METHODNAME(del)(node);

	if (!dq->head)
		dq->tail = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}

/**
 * @brief pop a node from the tail of a deque.
 * @param dq the deque to operate on.
 *
 * @returns the data of the popped node, 0 if dq or head is NULL.
 */
DEQUE_DATA_TYPE DEQUE_METHODNAME(pop_tail)(DEQUE_STRUCT_TAG *const restrict dq)
{
	DEQUE_DATA_TYPE zero;

	memset(&zero, 0, sizeof(zero));
	if (!dq || !dq->tail)
		return (zero);

	LISTNODE_STRUCT_TAG *node = dq->tail;

	dq->tail = LISTNODE_METHODNAME(get_prev)(node);
	DEQUE_DATA_TYPE d = LISTNODE_METHODNAME(del)(node);

	if (!dq->tail)
		dq->head = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}

/**
 * @brief create a new deque from an array of objects.
 * @param array pointer to the array of objects.
 * @param len number of items in the array.
 * @param data_dup function that can duplicate the objects.
 * @param data_free function that delete the delete objects,
 * should always be specified if `data_dup` is specified.
 *
 * @returns pointer to the new deque, NULL on error.
 */
DEQUE_STRUCT_TAG *DEQUE_METHODNAME(from_array)(
	DEQUE_DATA_TYPE *const restrict array, const len_ty len,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * data_dup,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * data_free
)
{
	if (!array || len < 1)
		return (NULL);

	/* A deletion function is required so as to free all allocated memory on failure. */
	if (data_dup && !data_free)
		return (NULL);

	DEQUE_STRUCT_TAG *restrict new_q = DEQUE_METHODNAME(new)();

	if (!new_q)
		return (NULL);

	for (len_ty i = 0; i < len; ++i)
	{
		DEQUE_DATA_TYPE data = array[i];

		if (!DEQUE_METHODNAME(push_tail)(new_q, data, data_dup))
		{
			new_q = DEQUE_METHODNAME(del)(new_q, data_free);
			break;
		}
	}

	return (new_q);
}

/**
 * @brief create an array from a deque.
 * @param dq the deque.
 * @param data_dup function that can duplicate the objects.
 * @param data_free function that delete the delete objects,
 * should always be specified if `data_dup` is specified.
 *
 * @returns pointer to the data array on success, NULL on failure.
 */
DEQUE_DATA_TYPE *DEQUE_METHODNAME(to_array)(
	const DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * data_dup,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * data_free
)
{
	if (!dq || !dq->head || dq->len < 1)
		return (NULL);

	/* A deletion function is required so as to free all allocated memory on failure. */
	if (data_dup && !data_free)
		return (NULL);

	DEQUE_DATA_TYPE *const restrict array =
		xmalloc(sizeof(*array) * (dq->len + 1));

	if (!array)
		return (NULL);

	LISTNODE_STRUCT_TAG *node = dq->head;
	for (size_t i = 0; node; node = LISTNODE_METHODNAME(get_next)(node), ++i)
	{
		DEQUE_DATA_TYPE data = LISTNODE_METHODNAME(get_data)(node);

		array[i] = data;
		if (data_dup)
		{
			if (data_dup(&array[i], data) == false)
			{
				for (size_t k = 0; k < i; k++)
					data_free(array[k]);

				xfree(array);
				return (NULL);
			}
		}
	}

	memset(&array[dq->len], 0, sizeof(*array));
	return (array);
}

/**
 * @brief stringify a deque.
 * @param dq the deque to print.
 * @param data_tostr function that will be called to print data in nodes.
 *
 * @returns number of bytes printed, negative number on error.
 */
char *DEQUE_METHODNAME(tostr)(
	const DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(stringify_data_, DEQUE_UNIQUE_SUFFIX) * data_tostr
)
{
	if (!dq)
		return (NULL);

	if (!dq->head)
	{
		char *const restrict s = xmalloc(sizeof("(NULL)"));

		if (s)
			strcpy(s, "(NULL)");

		return (s);
	}

	return (LIST_METHODNAME(tostr)(dq->head, data_tostr));
}

/**
 * @brief stringify a deque from tail to head.
 * @param dq the deque to print.
 * @param data_tostr function that will be called to print data in nodes.
 *
 * @returns number of bytes printed, negative number on error.
 */
char *DEQUE_METHODNAME(tostr_reversed)(
	const DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(stringify_data_, DEQUE_UNIQUE_SUFFIX) * data_tostr
)
{
	if (!dq)
		return (NULL);

	if (!dq->tail)
	{
		char *const restrict s = xmalloc(sizeof("(NULL)"));

		if (s)
			strcpy(s, "(NULL)");

		return (s);
	}

	return (LIST_METHODNAME(tostr_reversed)(dq->tail, data_tostr));
}

#undef LISTNODE_UNIQUE_SUFFIX
#undef LISTNODE_DATA_TYPE

#undef LISTNODE_STRUCT_TAG
#undef LISTNODE_METHODNAME
#undef LIST_METHODNAME

#undef DEQUE_UNIQUE_SUFFIX
#undef DEQUE_DATA_TYPE

#undef DQ_CONCAT_PLAIN
#undef DQ_CONCAT

#undef DEQUE_STRUCT_TAG
#undef DEQUE_METHODNAME
