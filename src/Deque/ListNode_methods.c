#ifndef LISTNODE_UNIQUE_SUFFIX
	#error "Missing definition for `LISTNODE_UNIQUE_SUFFIX`."
#endif

#ifndef LISTNODE_DATA_TYPE
	#error "Missing definition for `LISTNODE_DATA_TYPE`."
#endif

#include <assert.h> /* asserts */
#include <string.h> /* strlen, strcat */

#include "len_type.h"
#include "xalloc.h"

#define LN_CONCAT_PLAIN(tok0, tok1) tok0##tok1
#define LN_CONCAT(tok0, tok1) LN_CONCAT_PLAIN(tok0, tok1)

#define LISTNODE_STRUCT_TAG LN_CONCAT(ListNode_, LISTNODE_UNIQUE_SUFFIX)

#define LISTNODE_METHODNAME(name)                                             \
	LN_CONCAT(LN_CONCAT(ln_, LISTNODE_UNIQUE_SUFFIX), LN_CONCAT(_, name))

#define LIST_METHODNAME(name)                                                 \
	LN_CONCAT(LN_CONCAT(list_, LISTNODE_UNIQUE_SUFFIX), LN_CONCAT(_, name))

/**
 * @brief get next node.
 * @param node non-null pointer to a `LISTNODE_STRUCT_TAG`.
 *
 * @returns pointer to the next node.
 */
LISTNODE_STRUCT_TAG *
LISTNODE_METHODNAME(get_next)(const LISTNODE_STRUCT_TAG *const restrict node)
{
	assert(node);
	return (node->next);
}

/**
 * @brief get previous node.
 * @param node non-null pointer to a `LISTNODE_STRUCT_TAG`.
 *
 * @returns pointer to the previous node.
 */
LISTNODE_STRUCT_TAG *
LISTNODE_METHODNAME(get_prev)(const LISTNODE_STRUCT_TAG *const restrict node)
{
	assert(node);
	return (node->prev);
}

/**
 * @brief get data of a node.
 * @param node non-null pointer to a `LISTNODE_STRUCT_TAG`.
 *
 * @returns the data in the node.
 */
LISTNODE_DATA_TYPE
LISTNODE_METHODNAME(get_data)(const LISTNODE_STRUCT_TAG *const restrict node)
{
	assert(node);
	return (node->data);
}

/**
 * @brief set the next pointer of a node.
 * @param this_node non-null pointer to a `LISTNODE_STRUCT_TAG`.         \
 * @param new_node pointer to insert.
 *
 * @returns the old pointer.
 */
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(set_next)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict new_node
)
{
	assert(this_node);
	LISTNODE_STRUCT_TAG *const old_node = this_node->next;

	this_node->next = new_node;
	return (old_node);
}

/**
 * @brief set the previous pointer of a node.
 * @param this_node non-null pointer to a `LISTNODE_STRUCT_TAG`.         \
 * @param new_node pointer to insert.
 *
 * @returns the old pointer.
 */
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(set_prev)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict new_node
)
{
	assert(this_node);
	LISTNODE_STRUCT_TAG *const old_node = this_node->prev;

	this_node->prev = new_node;
	return (old_node);
}

/**
 * @brief set the data of a node.
 * @param this_node non-null pointer to a `LISTNODE_STRUCT_TAG`.         \
 * @param new_data the data to swap in.
 *
 * @returns the old data.
 */
LISTNODE_DATA_TYPE LISTNODE_METHODNAME(set_data)(
	LISTNODE_STRUCT_TAG *const restrict this_node, LISTNODE_DATA_TYPE new_data
)
{
	assert(this_node);
	LISTNODE_DATA_TYPE old_data = this_node->data;

	this_node->data = new_data;
	return (old_data);
}

/**
 * @brief allocate and initialise memory for a `LISTNODE_STRUCT_TAG`.    \
 * @param data the data to be stored in the new node.
 * @param data_copy pointer to a function that can duplicate `data`.
 *
 * @returns pointer to the created node, NULL on failure.
 */
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(new)(
	LISTNODE_DATA_TYPE data,
	LN_CONCAT(duplicate_, LISTNODE_UNIQUE_SUFFIX) * data_copy
)
{
	LISTNODE_STRUCT_TAG *const restrict node =
		xcalloc(1, sizeof(LISTNODE_STRUCT_TAG));

	if (!node)
		return (NULL);

	if (data_copy)
	{
		LISTNODE_DATA_TYPE cpy;

		if (data_copy(&cpy, data) == false)
		{
			xfree(node);
			return (NULL);
		}

		data = cpy;
	}

	LISTNODE_METHODNAME(set_data)(node, data);
	return (node);
}

/**
 * @brief unlink a node from a linked list.
 * @param node pointer to a `LISTNODE_STRUCT_TAG`.
 *
 * @returns pointer to the popped node.
 */
LISTNODE_STRUCT_TAG *
LISTNODE_METHODNAME(pop)(LISTNODE_STRUCT_TAG *const restrict node)
{
	if (!node)
		return (NULL);

	LISTNODE_STRUCT_TAG *const restrict next =
		LISTNODE_METHODNAME(set_next)(node, NULL);
	LISTNODE_STRUCT_TAG *const restrict prev =
		LISTNODE_METHODNAME(set_prev)(node, NULL);

	if (next)
		LISTNODE_METHODNAME(set_prev)(next, prev);

	if (prev)
		LISTNODE_METHODNAME(set_next)(prev, next);

	return (node);
}

/**
 * @brief pop and free a node from a linked list.
 * @param node pointer to a `LISTNODE_STRUCT_TAG`.
 *
 * @returns the data in the node, 0 if `node` is NULL.
 */
LISTNODE_DATA_TYPE
LISTNODE_METHODNAME(del)(LISTNODE_STRUCT_TAG *const restrict node)
{
	LISTNODE_DATA_TYPE zero;

	memset(&zero, 0, sizeof(zero));
	if (!node)
		return (zero);

	LISTNODE_DATA_TYPE data = LISTNODE_METHODNAME(get_data)(node);

	xfree(LISTNODE_METHODNAME(pop)(node));
	return (data);
}

/**
 * @brief insert a node after another node.
 * @param this_node pointer to a node.
 * @param other_node pointer to the node to insert.
 *
 * @returns pointer to the newly inserted node, NULL if `other_node` is NULL. \
 */
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(insert_after)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	LISTNODE_STRUCT_TAG *const restrict this_next =
		LISTNODE_METHODNAME(set_next)(this_node, other_node);

	if (this_next)
		LISTNODE_METHODNAME(set_prev)(this_next, other_node);

	LISTNODE_METHODNAME(set_next)(other_node, this_next);
	LISTNODE_METHODNAME(set_prev)(other_node, this_node);
	return (other_node);
}

/**
 * @brief insert a node before another node.
 * @param this_node pointer to a node.
 * @param other_node pointer to the node to insert.
 *
 * @returns pointer to the newly inserted node, NULL if `other_node` is NULL. \
 */
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(insert_before)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	LISTNODE_STRUCT_TAG *const restrict this_prev =
		LISTNODE_METHODNAME(set_prev)(this_node, other_node);

	if (this_prev)
		LISTNODE_METHODNAME(set_next)(this_prev, other_node);

	LISTNODE_METHODNAME(set_next)(other_node, this_node);
	LISTNODE_METHODNAME(set_prev)(other_node, this_prev);
	return (other_node);
}

/**
 * @brief swap data of two nodes.
 * @param this_node pointer to the node to be swapped out.
 * @param other_node pointer to the node to be swapped in.
 *
 * @returns pointer to the swapped in node, NULL if `other_node` is NULL.     \
 */
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(swap)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	LISTNODE_METHODNAME(set_data)(
		other_node, LISTNODE_METHODNAME(set_data)(
						this_node, LISTNODE_METHODNAME(get_data)(other_node)
					)
	);
	return (other_node);
}

/**
 * @brief free a linked list.
 * @param head pointer to the start of the linked list.
 * @param data_free function that will be called to free data in the nodes.   \
 *
 * @returns NULL always.
 */
void *LIST_METHODNAME(del)(
	LISTNODE_STRUCT_TAG *const head,
	LN_CONCAT(free_mem_, LISTNODE_UNIQUE_SUFFIX) * data_free
)
{
	if (!head)
		return (NULL);

	LISTNODE_STRUCT_TAG *walk = head;

	while (walk)
	{
		LISTNODE_STRUCT_TAG *next = LISTNODE_METHODNAME(get_next)(walk);
		LISTNODE_DATA_TYPE data = LISTNODE_METHODNAME(del)(walk);

		if (data_free)
			data_free(data);

		walk = next;
	}

	return (NULL);
}

/**
 * @brief stringify a linked list.
 * @param head pointer to the start of the linked list to stringify.
 * @param data_tostr pointer to a function that formats data in the nodes     \
 * into a string.
 *
 * @returns pointer to the stringified linked list, NULL on error.
 */
char *LIST_METHODNAME(tostr)(
	LISTNODE_STRUCT_TAG const *const restrict head,
	LN_CONCAT(stringify_data_, LISTNODE_UNIQUE_SUFFIX) * data_tostr
)
{
	if (!head || !data_tostr)
		return (NULL);

	const char link[] = " <--> ";
	char *restrict list_str = data_tostr(LISTNODE_METHODNAME(get_data)(head));

	if (!list_str)
		return (NULL);

	len_ty list_str_len = strlen(list_str);
	const LISTNODE_STRUCT_TAG *restrict walk =
		LISTNODE_METHODNAME(get_next)(head);

	while (list_str && walk)
	{
		char *restrict data_str =
			data_tostr(LISTNODE_METHODNAME(get_data)(walk));

		if (!data_str)
			break;

		list_str_len += sizeof(link) - 1 + strlen(data_str);
		list_str = xrealloc(list_str, list_str_len + 1);
		if (list_str)
			strcat(strcat(list_str, link), data_str);

		xfree(data_str);
		walk = LISTNODE_METHODNAME(get_next)(walk);
	}

	if (walk)
	{
		xfree(list_str);
		list_str = NULL;
	}

	return (list_str);
}

/**
 * @brief stringify a linked list in reverse.
 * @param tail pointer to the tail of the linked list.
 * @param data_tostr pointer to a function that formats data in the nodes     \
 * into a string.
 *
 * @returns pointer to the stringified linked list, NULL on error.
 */
char *LIST_METHODNAME(tostr_reversed)(
	LISTNODE_STRUCT_TAG const *const restrict tail,
	LN_CONCAT(stringify_data_, LISTNODE_UNIQUE_SUFFIX) * data_tostr
)
{
	if (!tail || !data_tostr)
		return (NULL);

	const char link[] = " <--> ";
	char *restrict list_str = data_tostr(LISTNODE_METHODNAME(get_data)(tail));

	if (!list_str)
		return (NULL);

	len_ty list_str_len = strlen(list_str);
	const LISTNODE_STRUCT_TAG *restrict walk =
		LISTNODE_METHODNAME(get_prev)(tail);

	while (list_str && walk)
	{
		LISTNODE_DATA_TYPE d = LISTNODE_METHODNAME(get_data)(walk);
		char *restrict data_str = data_tostr(d);

		if (!data_str)
			break;

		list_str_len += sizeof(link) - 1 + strlen(data_str);
		list_str = xrealloc(list_str, list_str_len + 1);
		if (list_str)
			strcat(strcat(list_str, link), data_str);

		xfree(data_str);
		walk = LISTNODE_METHODNAME(get_prev)(walk);
	}

	if (walk)
	{
		xfree(list_str);
		list_str = NULL;
	}

	return (list_str);
}

#undef LISTNODE_UNIQUE_SUFFIX
#undef LISTNODE_DATA_TYPE

#undef LN_CONCAT_PLAIN
#undef LN_CONCAT

#undef LISTNODE_STRUCT_TAG
#undef LISTNODE_METHODNAME
#undef LIST_METHODNAME
