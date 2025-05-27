#include <string.h> /* strcpy */

#include "deque.h"
#include "list_node.h"
#include "list_type_structs.h"
#include "xalloc.h"

/**
 * dq_new - allocate and initialise memory for a `deque`.
 *
 * Return: pointer to the new list.
 */
deque *dq_new(void) { return (xcalloc(1, sizeof(deque))); }

/**
 * dq_clear - free all the nodes of a `deque`.
 * @dq: the `deque` to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
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

/**
 * dq_del - frees a `deque` from memory.
 * @dq: pointer to the `deque` to delete.
 * @free_data: pointer to a function that can free data in the `deque`.
 *
 * Return: NULL always.
 */
void *dq_del(deque *const dq, free_func *free_data)
{
	dq_clear(dq, free_data);
	free(dq);
	return (NULL);
}

/**
 * dq_push_head - add a new node to the head of the `deque`.
 * @dq: the `deque` to operate on.
 * @data: data that the new node will hold.
 * @copy_data: function that will be called to duplicate `data`.
 *
 * Return: pointer to the new node, NULL on failure.
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

/**
 * dq_push_tail - add a node to the end of a `deque`.
 * @dq: the `deque` to operate on.
 * @data: data that the node will hold.
 * @copy_data: function that returns a separate copy of data,
 * if NULL a simple copy of the pointer to data is done.
 *
 * Return: pointer to the newly added node, NULL if dq is NULL or failure.
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

/**
 * dq_pop_head - pop a node from the head of a `deque` and return its data.
 * @dq: the `deque` to operate on.
 *
 * Return: pointer to the data in the popped node, NULL if dq or head is NULL.
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

/**
 * dq_pop_tail - pop a node from the tail of a `deque`.
 * @dq: the `deque` to operate on.
 *
 * Return: pointer to the data that was in the node.
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

/**
 * dq_tostr - stringify a `deque`.
 * @dq: the `deque` to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, negative number on error.
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

/**
 * dq_tostr_reversed - stringify a `deque` from tail to head.
 * @dq: the `deque` to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: number of bytes printed, negative number on error.
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
