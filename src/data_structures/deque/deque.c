/*!
 * @file
 * @brief methods for deque types.
 */

#include <string.h> /* strcpy */

#include "deque.h"
#include "list_node.h"
#include "list_type_structs.h"
#include "xalloc.h"

/*!
 * @brief allocate and initialise memory for a `deque`.
 * @public @memberof deque
 *
 * @return pointer to the new list.
 */
deque *dq_new(void) { return (xcalloc(1, sizeof(deque))); }

/*!
 * @brief free all the nodes of a `deque`.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to operate on.
 * @param[in] free_data pointer to a function that will be called to free data in nodes.
 */
void dq_clear(deque *const restrict dq, free_func *free_data)
{
	if (!dq || !dq->head)
		return;

	list_node *next_node = lstnode_get_next(dq->head);

	while (dq->head)
	{
		void *d = lstnode_del(dq->head);

		if (free_data)
			free_data(d);

		dq->head = next_node;
		next_node = dq->head ? lstnode_get_next(dq->head) : NULL;
	}

	dq->head = NULL;
	dq->tail = NULL;
	dq->len = 0;
}

/*!
 * @brief free a `deque` from memory.
 * @public @memberof deque
 *
 * @param[in] dq pointer to the `deque` to delete.
 * @param[in] free_data pointer to a function that can free data in the `deque`.
 *
 * @return NULL always.
 */
void *dq_del(deque *const restrict dq, free_func *free_data)
{
	dq_clear(dq, free_data);
	free(dq);
	return (NULL);
}

/*!
 * @brief add a new node to the head of the `deque`.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to operate on.
 * @param[in] data data that the new node will hold.
 * @param[in] copy_data function that will be called to duplicate `data`.
 *
 * @return pointer to the new node, NULL on failure.
 */
list_node *
dq_push_head(deque *const restrict dq, void *const data, dup_func *copy_data)
{
	if (!dq)
		return (NULL);

	list_node *const restrict nw = lstnode_new(data, copy_data);

	if (!nw)
		return (NULL);

	dq->head = lstnode_insert_before(dq->head, nw);
	if (!dq->tail)
		dq->tail = nw;

	++(dq->len);
	return (nw);
}

/*!
 * @brief add a node to the end of a `deque`.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to operate on.
 * @param[in] data data that the node will hold.
 * @param[in] copy_data function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * @return pointer to the newly added node, NULL if dq is NULL or failure.
 */
list_node *dq_push_tail(
	deque *const restrict dq, void *const restrict data, dup_func *copy_data
)
{
	if (!dq)
		return (NULL);

	list_node *const restrict nw = lstnode_new(data, copy_data);
	if (!nw)
		return (NULL);

	dq->tail = lstnode_insert_after(dq->tail, nw);
	if (!dq->head)
		dq->head = nw;

	++(dq->len);
	return (nw);
}

/*!
 * @brief pop a node from the head of a `deque` and return its data.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to operate on.
 *
 * @return pointer to the data in the popped node, NULL if dq or head is NULL.
 */
void *dq_pop_head(deque *const restrict dq)
{
	if (!dq || !dq->head)
		return (NULL);

	list_node *const node = dq->head;

	dq->head = lstnode_get_next(node);
	void *const d = lstnode_del(node);

	if (!dq->head)
		dq->tail = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}

/*!
 * @brief pop a node from the tail of a `deque`.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to operate on.
 *
 * @return pointer to the data that was in the node.
 */
void *dq_pop_tail(deque *const restrict dq)
{
	if (!dq || !dq->tail)
		return (NULL);

	list_node *node = dq->tail;

	dq->tail = lstnode_get_prev(node);
	void *const d = lstnode_del(node);

	if (!dq->tail)
		dq->head = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}

/*!
 * @brief stringify a `deque`.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to print.
 * @param[in] print_data function that will be called to print data in nodes.
 *
 * @return number of bytes printed, negative number on error.
 */
char *dq_tostr(deque const *const restrict dq, data_tostr *print_data)
{
	if (!dq)
		return (NULL);

	if (!dq->head)
	{
		char *const restrict s = malloc(sizeof("(NULL)"));

		if (s)
			strcpy(s, "(NULL)");

		return (s);
	}

	return (linked_list_tostr(dq->head, print_data));
}

/*!
 * @brief stringify a `deque` from tail to head.
 * @public @memberof deque
 *
 * @param[in] dq the `deque` to print.
 * @param[in] print_data function that will be called to print data in nodes.
 *
 * @return number of bytes printed, negative number on error.
 */
char *dq_tostr_reversed(deque const *const restrict dq, data_tostr *print_data)
{
	if (!dq)
		return (NULL);

	if (!dq->tail)
	{
		char *const restrict s = malloc(sizeof("(NULL)"));

		if (s)
			strcpy(s, "(NULL)");

		return (s);
	}

	return (linked_list_tostr_reversed(dq->tail, print_data));
}
