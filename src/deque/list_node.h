#ifndef DS_LIST_NODE_TYPE_H
#define DS_LIST_NODE_TYPE_H

#include "attribute_macros.h"
#include "list_type_typedefs.h"

/* alloc and free */

list_node *lstnode_pop(list_node *const restrict node);
void *lstnode_del(list_node *const restrict node);
list_node *
lstnode_new(void *data, dup_func *copy_func) _malloc _malloc_free(lstnode_del);

void *linked_list_del(list_node *const head, free_func *delete_func);

/* access */

list_node *lstnode_get_next(const list_node *const node) _nonnull;
list_node *lstnode_get_prev(const list_node *const node) _nonnull;
void *lstnode_get_data(const list_node *const node) _nonnull;

/* manipulate */

list_node *lstnode_set_next(
	list_node *const restrict this_node, list_node *const restrict new_node
) _nonnull_pos(1);
list_node *lstnode_set_prev(
	list_node *const restrict this_node, list_node *const restrict new_node
) _nonnull_pos(1);
void *lstnode_set_data(
	list_node *const restrict node, void *const restrict new_data
);

list_node *lstnode_insert_after(
	list_node *const restrict this_node, list_node *const restrict other_node
);
list_node *lstnode_insert_before(
	list_node *const restrict this_node, list_node *const restrict other_node
);
list_node *lstnode_swap(
	list_node *const restrict this_node, list_node *const restrict other_node
);

/* print */

char *linked_list_tostr(
	list_node const *const restrict tail, data_tostr *stringify_func
) _malloc;
char *linked_list_tostr_reversed(
	list_node const *const restrict tail, data_tostr *stringify_func
) _malloc;

#endif  // DS_LIST_NODE_TYPE_H
