#include "linked_node.h"
#include "list_type_structs.h"

/**
 * node_get_next - get next node.
 * @node: pointer to a `linked_node`, should not be NULL.
 *
 * Return: pointer to the next node.
 */
linked_node *node_get_next(const linked_node *const node)
{
	assert(node);
	return (node->next);
}

/**
 * node_get_prev - get previous node.
 * @node: pointer to a `linked_node`, should not be NULL.
 *
 * Return: pointer to the previous node.
 */
linked_node *node_get_prev(const linked_node *const node)
{
	assert(node);
	return (node->prev);
}

/**
 * node_get_data - get data of a node.
 * @node: pointer to a `linked_node`, should not be NULL.
 *
 * Return: pointer to the data in the node.
 */
void *node_get_data(const linked_node *const node)
{
	assert(node);
	return (node->data);
}

/**
 * node_set_next - swap out the next node of a another node.
 * @this_node: pointer to a `linked_node` to be modified, should not be NULL.
 * @other_node: pointer to the `linked_node` to be swapped in.
 *
 * Return: pointer to the old next node.
 */
linked_node *node_set_next(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
)
{
	assert(this_node);
	linked_node *const tmp = this_node->next;

	this_node->next = other_node;
	return (tmp);
}

/**
 * node_set_prev - swap out the previous node of a another node.
 * @this_node: pointer to a `linked_node` to be modified, should not be NULL.
 * @other_node: pointer to the `linked_node` to be swapped in.
 *
 * Return: pointer to the old previous node.
 */
linked_node *node_set_prev(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
)
{
	assert(this_node);
	linked_node *const tmp = this_node->prev;

	this_node->prev = other_node;
	return (tmp);
}

/**
 * node_set_data - swap the data of a node with new data.
 * @node: pointer to a `linked_node` to modify, should not be NULL.
 * @data: pointer to the data to swap in.
 *
 * Return: pointer to the old data.
 */
void *
node_set_data(linked_node *const restrict node, void *const restrict data)
{
	assert(node);
	void *const tmp = node->data;

	node->data = data;
	return (tmp);
}

/**
 * node_new - allocate memory for a new `linked_node` and initialise it.
 * @data: pointer to the data to be stored in the new node.
 * @copy_data: pointer to a function that can duplicate `data`.
 *
 * Return: pointer to the created node, NULL on failure.
 */
linked_node *node_new(void *data, dup_func *copy_data)
{
	linked_node *node = calloc(1, sizeof(linked_node));

	if (!node)
		return (NULL);

	node_set_data(node, data);
	if (copy_data)
	{
		node_set_data(node, copy_data(data));
		if (!node_get_data(node) && data)
		{
			free(node);
			node = NULL;
		}
	}

	return (node);
}

/**
 * node_pop - remove a node from a linked list.
 * @node: pointer to a `linked_node`.
 *
 * Return: pointer to the popped linked_node.
 */
linked_node *node_pop(linked_node *const restrict node)
{
	if (!node)
		return (NULL);

	linked_node *const restrict next = node_set_next(node, NULL);
	linked_node *const restrict prev = node_set_prev(node, NULL);

	if (next)
		node_set_prev(next, prev);

	if (prev)
		node_set_next(prev, next);

	return (node);
}

/**
 * node_del - pop and free a `linked_node`.
 * @node: pointer to a `linked_node`.
 *
 * Return: pointer to the data in the node.
 */
void *node_del(linked_node *const restrict node)
{
	if (!node)
		return (NULL);

	void *const data = node_set_data(node, NULL);

	free(node_pop(node));
	return (data);
}

/**
 * node_insert_after - insert a node after another node.
 * @this_node: pointer to a node.
 * @other_node: pointer to the node to be inserted.
 *
 * Return: pointer to the newly inserted node, NULL if `other_node` is NULL.
 */
linked_node *node_insert_after(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	linked_node *const restrict this_next =
		node_set_next(this_node, other_node);
	if (this_next)
		node_set_prev(this_next, other_node);

	node_set_next(other_node, this_next);
	node_set_prev(other_node, this_node);
	return (other_node);
}

/**
 * node_insert_before - insert a node before another node.
 * @this_node: pointer to a node.
 * @other_node: pointer to the node to be inserted.
 *
 * Return: pointer to the newly inserted node, NULL if `other_node` is NULL.
 */
linked_node *node_insert_before(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	linked_node *const restrict this_prev =
		node_set_prev(this_node, other_node);
	if (this_prev)
		node_set_next(this_prev, other_node);

	node_set_next(other_node, this_node);
	node_set_prev(other_node, this_prev);
	return (other_node);
}

/**
 * node_swap - swap data of two nodes.
 * @this_node: pointer to the node to be swapped out.
 * @other_node: pointer to the node to be swapped in.
 *
 * Return: pointer to the swapped in node, NULL if `other_node` is NULL.
 */
linked_node *node_swap(
	linked_node *const restrict this_node,
	linked_node *const restrict other_node
)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (NULL);

	node_set_data(
		other_node, node_set_data(this_node, node_get_data(other_node))
	);
	return (other_node);
}

/**
 * linked_list_del - free a linked list.
 * @head: pointer to the start of the linked list.
 * @free_data: function that will be called to free data in the nodes.
 *
 * Return: NULL always.
 */
void *linked_list_del(linked_node *const head, free_func *free_data)
{
	if (!head)
		return (NULL);

	linked_node *restrict walk = head, *next = node_get_next(head);

	while (walk)
	{
		void *data = node_del(walk);
		if (free_data)
			free_data(data);

		walk = next;
		next = walk ? node_get_next(walk) : NULL;
	}

	return (NULL);
}
