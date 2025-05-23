#include "deque.h"
#include "linked_node.h"
#include "list_type_structs.h"

/**
 * dq_new - allocate and initialise memory for a `deque`.
 *
 * Return: pointer to the new list.
 */
deque *dq_new(void) { return (calloc(1, sizeof(deque))); }

/**
 * dq_clear - free all the nodes of a `deque`.
 * @dq: the `deque` to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 */
void dq_clear(deque *const restrict dq, free_func *free_data)
{
	if (!dq || !dq->head)
		return;

	linked_node *next_node = node_get_next(dq->head);

	while (dq->head)
	{
		void *d = node_del(dq->head);

		if (free_data)
			free_data(d);

		dq->head = next_node;
		next_node = dq->head ? node_get_next(dq->head) : NULL;
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
linked_node *
dq_push_head(deque *const restrict dq, void *const data, dup_func *copy_data)
{
	if (!dq)
		return (NULL);

	linked_node *const restrict nw = node_new(data, copy_data);

	if (!nw)
		return (NULL);

	dq->head = node_insert_before(dq->head, nw);
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
linked_node *dq_push_tail(
	deque *const restrict dq, void *const restrict data, dup_func *copy_data
)
{
	if (!dq)
		return (NULL);

	linked_node *const restrict nw = node_new(data, copy_data);
	if (!nw)
		return (NULL);

	dq->tail = node_insert_after(dq->tail, nw);
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

	linked_node *const node = dq->head;

	dq->head = node_get_next(node);
	void *const d = node_del(node);

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

	linked_node *node = dq->tail;

	dq->tail = node_get_prev(node);
	void *const d = node_del(node);

	if (!dq->tail)
		dq->head = NULL;

	if (dq->len > 0)
		--(dq->len);

	return (d);
}
