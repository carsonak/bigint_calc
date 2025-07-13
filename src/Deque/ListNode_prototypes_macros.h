#ifndef DS_LISTNODE_PROTOTYPES_MACROS_H
#define DS_LISTNODE_PROTOTYPES_MACROS_H

#include "common_callbacks_macros.h"
#include "compiler_attributes_macros.h"

/*!
 * @brief define prototypes for methods for a ListNode type.
 * @param unique_suffix a unique suffix for method names and ListNode type.
 * @param data_type the type of objects the nodes store.
 */
#define DECLARE_LISTNODE_PROTOTYPES(unique_suffix, data_type)                 \
	DECLARE_COMMON_CALLBACKS(unique_suffix, data_type)                        \
                                                                              \
	typedef struct ListNode_##unique_suffix ListNode_##unique_suffix;         \
                                                                              \
	/* alloc and free */                                                      \
                                                                              \
	ListNode_##unique_suffix *ln_##unique_suffix##_pop(                       \
		ListNode_##unique_suffix *const restrict node                         \
	);                                                                        \
	data_type ln_##unique_suffix##_del(                                       \
		ListNode_##unique_suffix *const restrict node                         \
	);                                                                        \
	ListNode_##unique_suffix *ln_##unique_suffix##_new(                       \
		data_type data, duplicate_##unique_suffix *copy_data                  \
	) _malloc _malloc_free(ln_##unique_suffix##_del);                         \
                                                                              \
	void *list_##unique_suffix##_del(                                         \
		ListNode_##unique_suffix *const head,                                 \
		free_mem_##unique_suffix *free_data                                   \
	);                                                                        \
                                                                              \
	/* access */                                                              \
                                                                              \
	ListNode_##unique_suffix *ln_##unique_suffix##_get_next(                  \
		const ListNode_##unique_suffix *const restrict node                   \
	) _nonnull;                                                               \
	ListNode_##unique_suffix *ln_##unique_suffix##_get_prev(                  \
		const ListNode_##unique_suffix *const restrict node                   \
	) _nonnull;                                                               \
	data_type ln_##unique_suffix##_get_data(                                  \
		const ListNode_##unique_suffix *const restrict node                   \
	) _nonnull;                                                               \
                                                                              \
	/* manipulate */                                                          \
                                                                              \
	ListNode_##unique_suffix *ln_##unique_suffix##_set_next(                  \
		ListNode_##unique_suffix *const restrict this_node,                   \
		ListNode_##unique_suffix *const restrict other_node                   \
	) _nonnull_pos(1);                                                        \
	ListNode_##unique_suffix *ln_##unique_suffix##_set_prev(                  \
		ListNode_##unique_suffix *const restrict this_node,                   \
		ListNode_##unique_suffix *const restrict other_node                   \
	) _nonnull_pos(1);                                                        \
	data_type ln_##unique_suffix##_set_data(                                  \
		ListNode_##unique_suffix *const restrict node, data_type data         \
	);                                                                        \
                                                                              \
	ListNode_##unique_suffix *ln_##unique_suffix##_insert_after(              \
		ListNode_##unique_suffix *const restrict this_node,                   \
		ListNode_##unique_suffix *const restrict other_node                   \
	);                                                                        \
	ListNode_##unique_suffix *ln_##unique_suffix##_insert_before(             \
		ListNode_##unique_suffix *const restrict this_node,                   \
		ListNode_##unique_suffix *const restrict other_node                   \
	);                                                                        \
	ListNode_##unique_suffix *ln_##unique_suffix##_swap(                      \
		ListNode_##unique_suffix *const restrict this_node,                   \
		ListNode_##unique_suffix *const restrict other_node                   \
	);                                                                        \
                                                                              \
	/* print */                                                               \
                                                                              \
	char *list_##unique_suffix##_tostr(                                       \
		ListNode_##unique_suffix const *const restrict head,                  \
		stringify_data_##unique_suffix *data_tostr                            \
	) _malloc;                                                                \
	char *list_##unique_suffix##_tostr_reversed(                              \
		ListNode_##unique_suffix const *const restrict tail,                  \
		stringify_data_##unique_suffix *data_tostr                            \
	) _malloc;

#endif /* DS_LISTNODE_PROTOTYPES_MACROS_H */
