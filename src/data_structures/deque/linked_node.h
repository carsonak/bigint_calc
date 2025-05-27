#ifndef DS_LINKED_NODE_TYPE_H
#define DS_LINKED_NODE_TYPE_H

#include <assert.h> /* asserts */
#include <stdlib.h> /* *alloc */

#include "attribute_macros.h"
#include "list_type_typedefs.h"

list_node *lstnode_pop(list_node *const restrict node);
void *lstnode_del(list_node *const restrict node);
list_node *lstnode_new(
	void *data, dup_func *copy_data
) ATTR_MALLOC ATTR_MALLOC_FREE(lstnode_del);

list_node *lstnode_get_next(const list_node *const node) ATTR_NONNULL;
list_node *lstnode_get_prev(const list_node *const node) ATTR_NONNULL;
void *lstnode_get_data(const list_node *const node) ATTR_NONNULL;
list_node *lstnode_set_next(
	list_node *const restrict this_node, list_node *const restrict other_node
) ATTR_NONNULL_IDX(1);
list_node *lstnode_set_prev(
	list_node *const restrict this_node, list_node *const restrict other_node
) ATTR_NONNULL_IDX(1);
void *
lstnode_set_data(list_node *const restrict node, void *const restrict data);

list_node *lstnode_insert_after(
	list_node *const restrict this_node, list_node *const restrict other_node
);
list_node *lstnode_insert_before(
	list_node *const restrict this_node, list_node *const restrict other_node
);
list_node *lstnode_swap(
	list_node *const restrict this_node, list_node *const restrict other_node
);

void *linked_list_del(list_node *const head, free_func *free_data);

#endif /* DS_LINKED_NODE_TYPE_H */
