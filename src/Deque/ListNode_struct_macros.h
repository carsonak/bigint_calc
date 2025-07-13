#ifndef DS_LISTNODE_STRUCT_MACROS_H
#define DS_LISTNODE_STRUCT_MACROS_H

/*!
 * @brief define struct for a ListNode type.
 * @param unique_suffix a unique suffix for method names and ListNode type.
 * @param data_type the type of objects the nodes store.
 */
#define DEFINE_LISTNODE_STRUCT(unique_suffix, data_type)                      \
	/**                                                                       \
	 * @brief a doubly linked node.                                           \
	 * @param next pointer to the next node.                                  \
	 * @param prev pointer to the previous node.                              \
	 * @param data pointer to the data for the node.                          \
	 */                                                                       \
	struct ListNode_##unique_suffix                                           \
	{                                                                         \
		struct ListNode_##unique_suffix *restrict next;                       \
		struct ListNode_##unique_suffix *restrict prev;                       \
		data_type data;                                                       \
	};

#endif /* DS_LISTNODE_STRUCT_MACROS_H */
