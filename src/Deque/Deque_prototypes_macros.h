#ifndef DS_DEQUE_PROTOTYPES_MACROS_H
#define DS_DEQUE_PROTOTYPES_MACROS_H

#include <stdint.h> /* intmax_t */

#include "ListNode_prototypes_macros.h"
#include "compiler_attributes_macros.h"

/*!
 * @brief define prototype for methods for a Deque type.
 * @param unique_suffix a unique suffix for method names and Deque type.
 * @param data_type the type of objects the nodes store.
 */
#define DECLARE_DEQUE_PROTOTYPES(unique_suffix, data_type)                    \
	DECLARE_LISTNODE_PROTOTYPES(unique_suffix, data_type)                     \
                                                                              \
	typedef struct Deque_##unique_suffix Deque_##unique_suffix;               \
                                                                              \
	/* alloc and free */                                                      \
                                                                              \
	void *dq_##unique_suffix##_del(                                           \
		Deque_##unique_suffix *const restrict dq,                             \
		free_mem_##unique_suffix *free_data                                   \
	);                                                                        \
	Deque_##unique_suffix *dq_##unique_suffix##_new(void)                     \
		_malloc _malloc_free(dq_##unique_suffix##_del);                       \
                                                                              \
	/* manipulate */                                                          \
                                                                              \
	ListNode_##unique_suffix *dq_##unique_suffix##_push_head(                 \
		Deque_##unique_suffix *const restrict dq, data_type data,             \
		duplicate_##unique_suffix *copy_data                                  \
	);                                                                        \
	ListNode_##unique_suffix *dq_##unique_suffix##_push_tail(                 \
		Deque_##unique_suffix *const restrict dq, data_type data,             \
		duplicate_##unique_suffix *copy_data                                  \
	);                                                                        \
	data_type dq_##unique_suffix##_pop_head(                                  \
		Deque_##unique_suffix *const restrict dq                              \
	);                                                                        \
	data_type dq_##unique_suffix##_pop_tail(                                  \
		Deque_##unique_suffix *const restrict dq                              \
	);                                                                        \
	void dq_##unique_suffix##_clear(                                          \
		Deque_##unique_suffix *const restrict dq,                             \
		free_mem_##unique_suffix *free_data                                   \
	);                                                                        \
                                                                              \
	/* array conversion */                                                    \
                                                                              \
	Deque_##unique_suffix *dq_##unique_suffix##_from_array(                   \
		data_type *const restrict array, const intmax_t len,                  \
		duplicate_##unique_suffix *copy_data,                                 \
		free_mem_##unique_suffix *delete_data                                 \
	);                                                                        \
	data_type *dq_##unique_suffix##_to_array(                                 \
		const Deque_##unique_suffix *const restrict dq,                       \
		duplicate_##unique_suffix *copy_data,                                 \
		free_mem_##unique_suffix *free_data                                   \
	);                                                                        \
                                                                              \
	/* print */                                                               \
                                                                              \
	char *dq_##unique_suffix##_tostr(                                         \
		Deque_##unique_suffix const *const restrict dq,                       \
		stringify_data_##unique_suffix *print_data                            \
	);                                                                        \
	char *dq_##unique_suffix##_tostr_reversed(                                \
		const Deque_##unique_suffix *const restrict dq,                       \
		stringify_data_##unique_suffix *print_data                            \
	);

#endif /* DS_DEQUE_PROTOTYPES_MACROS_H */
