#include "data_structures.h"

/**
 * push_head - push an item to the head of the deque.
 * @dq_head: the deque to operate on.
 * @data: the data that the node will hold.
 *
 * Return: pointer to the newly added node, NULL if dq_head is NULL or failure.
 */
struct double_link_node *push_head(struct deque *dq_head, void *data)
{
	struct double_link_node *new_node = NULL;

	if (!dq_head)
		return (NULL);

	new_node = xcalloc(1, sizeof(*new_node));
	if (!new_node)
		return (NULL);

	new_node->data = data;
	new_node->next = dq_head->head;
	if (dq_head->head)
		dq_head->head->prev = new_node;

	if (!dq_head->tail)
		dq_head->tail = new_node;

	dq_head->head = new_node;
	++(dq_head->size);
	return (new_node);
}

/**
 * push_tail - push an item to the tail of the deque.
 * @dq_head: the deque to operate on.
 * @data: the data that the node will hold.
 *
 * Return: pointer to the newly added node, NULL if dq_head is NULL or failure.
 */
struct double_link_node *push_tail(struct deque *dq_head, void *data)
{
	struct double_link_node *new_node = NULL;

	if (!dq_head)
		return (NULL);

	new_node = xcalloc(1, sizeof(*new_node));
	if (!new_node)
		return (NULL);

	new_node->data = data;
	new_node->prev = dq_head->tail;
	if (dq_head->tail)
		dq_head->tail->next = new_node;

	if (!dq_head->head)
		dq_head->head = new_node;

	dq_head->tail = new_node;
	++(dq_head->size);
	return (new_node);
}

/**
 * pop_head - pops node at the head of the deque and returns its data.
 * @dq_head: the deque to operate on.
 *
 * Return: data that was in the popped node.
 */
void *pop_head(struct deque *dq_head)
{
	struct double_link_node *p = NULL;
	void *d = NULL;

	if (!dq_head || !dq_head->head)
		return (NULL);

	p = dq_head->head;
	d = dq_head->head->data;
	dq_head->head = dq_head->head->next;
	p = free_n_null(p);
	if (!dq_head->head)
		dq_head->tail = NULL;

	if (dq_head->size)
		dq_head->size--;

	return (d);
}

/**
 * pop_tail - pops node at the tail of the deque and returns its data.
 * @dq_head: the deque to operate on.
 *
 * Return: data that was in the popped node.
 */
void *pop_tail(struct deque *dq_head)
{
	struct double_link_node *p = NULL;
	void *d = NULL;

	if (!dq_head || !dq_head->tail)
		return (NULL);

	p = dq_head->tail;
	d = dq_head->tail->data;
	dq_head->tail = dq_head->tail->prev;
	p = free_n_null(p);
	if (!dq_head->tail)
		dq_head->head = NULL;

	if (dq_head->size)
		dq_head->size--;

	return (d);
}

/**
 * clear_deque - deletes a deque.
 * @dq: the deque to operate on.
 * @free_data: pointer to a function that will be called to free data in nodes.
 *
 * Return: NULL always.
 */
void *clear_deque(struct deque *dq, void *(*free_data)(void *))
{
	if (!dq || !dq->head)
		return (NULL);

	while (dq->head->next)
	{
		if (free_data)
			free_data(dq->head->data);

		dq->head = dq->head->next;
		dq->head->prev = free_n_null(dq->head->prev);
	}

	if (free_data)
		free_data(dq->head->data);

	dq->head = free_n_null(dq->head);
	dq->tail = NULL;
	dq->size = 0;
	return (NULL);
}

/**
 * print_deque - print all nodes of a deque.
 * @dq_head: the deque to print.
 * @print_data: function that will be called to print data in nodes.
 */
void print_deque(struct deque *dq_head, void (*print_data)(void *))
{
	struct double_link_node *walk = NULL;

	if (!dq_head)
		return;

	if (!dq_head->head)
		printf("(NULL)\n");

	if (print_data)
		(*print_data)(dq_head->head->data);
	else
		printf("%p", (void *)dq_head->head);

	walk = dq_head->head->next;
	while (walk)
	{
		if (print_data)
		{
			printf(" <=> ");
			(*print_data)(walk->data);
		}
		else
			printf(" <=> %p", walk->data);

		walk = walk->next;
	}

	printf("\n");
}
