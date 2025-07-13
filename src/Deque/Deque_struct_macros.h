#ifndef DS_DEQUE_STRUCT_MACROS_H
#define DS_DEQUE_STRUCT_MACROS_H

#include <stdint.h> /* intmax_t */

#include "ListNode_struct_macros.h"

/*!
 * @brief define struct for a Deque type.
 * @param unique_suffix a unique suffix for method names and Deque type.
 * @param data_type the type of objects the nodes store.
 */
#define DEFINE_DEQUE_STRUCT(unique_suffix, data_type)                         \
	DEFINE_LISTNODE_STRUCT(unique_suffix, data_type)                          \
                                                                              \
	/**                                                                       \
	 * @brief a `deque` type.                                                 \
	 * @param len number of nodes in the deque.                               \
	 * @param head pointer to the head node of the deque.                     \
	 * @param tail pointer to the tail node of the deque.                     \
	 */                                                                       \
	struct Deque_##unique_suffix                                              \
	{                                                                         \
		intmax_t len;                                                         \
		struct ListNode_##unique_suffix *head;                                \
		struct ListNode_##unique_suffix *tail;                                \
	};

#endif /* DS_DEQUE_STRUCT_MACROS_H */
