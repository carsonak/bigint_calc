#ifndef BIGINT_LIST_TYPE_STRUCTS_H
#define BIGINT_LIST_TYPE_STRUCTS_H

#include <stdint.h> /* intmax_t */

#include "list_type_typedefs.h"

/*!
 * @brief a double ended queue.
 */
struct deque
{
	/*! @public number of nodes in the deque. */
	intmax_t len;
	/*! @public pointer to the head node of the deque. */
	list_node *head;
	/*! @public pointer to the tail node of the deque. */
	list_node *tail;
};

/*!
 * @brief a doubly linked node.
 */
struct list_node
{
	/*! @protected pointer to the next node. */
	struct list_node *restrict next;
	/*! @protected pointer to the previous node. */
	struct list_node *restrict prev;
	/*! @protected pointer to the data for the node. */
	void *data;
};

#endif /* BIGINT_LIST_TYPE_STRUCTS_H */
