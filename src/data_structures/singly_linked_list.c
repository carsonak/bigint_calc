#include "linked_lists.h"

/**
 * struct single_link_node - a singly linked list node.
 * @data: data that the node holds.
 * @next: pointer to the next node.
 * @previous: pointer to the previous node.
 */
struct single_link_node
{
	void *data;
	struct single_link_node *next;
	/*This second pointer makes insertions and deletions easier.*/
	struct single_link_node *previous;
};

/**
 * sln_new - allocates and initialises memory for a `single_link_node`.
 * @data: pointer to the object to initialise with.
 * @duplicate_data: function that returns a separate copy of data,
 * if NULL the pointer to the original data is stored.
 *
 * Return: pointer to the new node, NULL on failure.
 */
single_link_node *sln_new(void *const data, dup_func *duplicate_data)
{
	single_link_node *new_node = calloc(1, sizeof(*new_node));

	if (!new_node)
		return (NULL);

	new_node->data = data;
	if (duplicate_data)
	{
		new_node->data = duplicate_data(data);
		if (!new_node->data && data)
		{
			free(new_node);
			return (NULL);
		}
	}

	return (new_node);
}

/**
 * sln_insert_after - inserts a `single_link_node` after another.
 * @this_node: the node to insert after.
 * @other_node: the node to insert.
 *
 * Return: pointer to the newly inserted node.
 */
single_link_node *sln_insert_after(
	single_link_node *const this_node, single_link_node *const other_node)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (this_node);

	other_node->previous = this_node;
	other_node->next = this_node->next;
	if (this_node->next)
		this_node->next->previous = other_node;

	this_node->next = other_node;
	return (other_node);
}

/**
 * sln_insert_before - insert a single link node before another.
 * @this_node: the node to insert before.
 * @other_node: the node to insert.
 *
 * Return: pointer to the newly inserted node.
 */
single_link_node *sln_insert_before(
	single_link_node *const this_node, single_link_node *other_node)
{
	if (!this_node)
		return (other_node);

	if (!other_node)
		return (this_node);

	other_node->next = this_node;
	other_node->previous = this_node->previous;
	if (this_node->previous)
		this_node->previous->next = other_node;

	this_node->previous = other_node;
	return (other_node);
}

/**
 * sln_remove - deletes a node and returns its data.
 * @node: the node to delete.
 *
 * Return: pointer to the data of the node.
 */
void *sln_remove(single_link_node *const node)
{
	if (!node)
		return (NULL);

	void *d = node->data;

	node->data = NULL;
	if (node->next)
		node->next->previous = node->previous;

	if (node->previous)
		node->previous->next = node->next;

	node->next = NULL;
	node->previous = NULL;
	free(node);
	return (d);
}

/**
 * sln_get_data - return the data in a single link node.
 * @node: the node.
 *
 * Return: pointer to the data.
 */
void *sln_get_data(single_link_node const *const node)
{
	if (!node)
		return (NULL);

	return (node->data);
}

/**
 * sln_swap_data - replace data in a node.
 * @node: pointer to the node to modify.
 * @data: pointer to the data to swap in.
 * @copy_data: function that will be called to duplicate data, if NULL
 * only pointer to `data` is stored.
 *
 * Return: pointer to the old data in node, NULL on failure.
 */
void *sln_swap_data(single_link_node *const node, void *const data, dup_func *copy_data)
{
	if (!node)
		return (NULL);

	void *old_data = sln_get_data(node);

	node->data = data;
	if (copy_data)
	{
		node->data = copy_data(data);
		if (!node->data && data)
		{
			node->data = old_data;
			return (NULL);
		}
	}

	return (old_data);
}

/**
 * sln_get_next - return a node's successor.
 * @node: the node to query.
 *
 * Return: pointer to the next node.
 */
single_link_node *sln_get_next(single_link_node const *const node)
{
	if (!node)
		return (NULL);

	return (node->next);
}

/**
 * sll_clear - delete a doubly linked list from memory.
 * @head: pointer to the head of the doubly linked list.
 * @free_data: function that will be called to free data in the nodes.
 *
 * Return: NULL always.
 */
void *sll_clear(single_link_node *const head, delete_func *free_data)
{
	if (!head)
		return (NULL);

	single_link_node *walk = head;
	void *data = NULL;

	while (walk->next)
	{
		walk = sln_get_next(walk);
		data = sln_remove(walk->previous);
		if (free_data)
			free_data(data);
	}

	data = sln_remove(walk);
	if (free_data)
		free_data(data);

	return (NULL);
}

/**
 * sll_print - print all nodes of a doubly linked list.
 * @stream: pointer to a stream to output to.
 * @head: head of the doubly linked list to print.
 * @print_data: function that will be called to print data in nodes.
 *
 * Return: total bytes printed, negative number on error.
 */
long int sll_print(FILE *stream, single_link_node const *const head, print_func *print_data)
{
	if (!stream || !head)
		return (-1);

	long int total_bytes = 0;
	int bytes_printed = 0;
	single_link_node const *walk = head;

	if (print_data)
		bytes_printed = print_data(stream, sln_get_data(walk));
	else
		bytes_printed = fprintf(stream, "%p", sln_get_data(walk));

	if (bytes_printed < 0)
		return (bytes_printed);

	total_bytes += bytes_printed;
	walk = sln_get_next(walk);
	while (walk)
	{
		bytes_printed = fprintf(stream, " --> ");
		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		if (print_data)
			bytes_printed = print_data(stream, sln_get_data(walk));
		else
			bytes_printed = fprintf(stream, "%p", sln_get_data(walk));

		if (bytes_printed < 0)
			return (bytes_printed);

		total_bytes += bytes_printed;
		walk = sln_get_next(walk);
	}

	bytes_printed = fprintf(stream, "\n");
	if (bytes_printed < 0)
		return (bytes_printed);

	return (total_bytes + bytes_printed);
}
