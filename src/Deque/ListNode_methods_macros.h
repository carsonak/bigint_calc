#ifndef LISTNODE_METHODS_MACROS_H
#define LISTNODE_METHODS_MACROS_H

/* vsnprintf */
#define _ISOC99_SOURCE

#include <assert.h> /* asserts */
#include <stdarg.h> /* va_arg, vsnprintf */
#include <stdint.h> /* intmax_t */
#include <stdio.h>  /* vsnprintf */
#include <stdlib.h> /* *alloc */
#include <string.h> /* strlen, strcat */

#include "xalloc.h"

/*!
 * @brief define methods for a ListNode type.
 * @param unique_suffix a unique suffix for method names and ListNode type.
 * @param data_type the type of objects the nodes store.
 */
#define DEFINE_LISTNODE_METHODS(unique_suffix, data_type)                         \
	/**                                                                           \
	 * @brief get next node.                                                      \
	 * @param node non-null pointer to a `ListNode_##unique_suffix`.              \
	 *                                                                            \
	 * @returns pointer to the next node.                                         \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_get_next(                      \
		const ListNode_##unique_suffix *const restrict node                       \
	)                                                                             \
	{                                                                             \
		assert(node);                                                             \
		return (node->next);                                                      \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief get previous node.                                                  \
	 * @param node non-null pointer to a `ListNode_##unique_suffix`.              \
	 *                                                                            \
	 * @returns pointer to the previous node.                                     \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_get_prev(                      \
		const ListNode_##unique_suffix *const restrict node                       \
	)                                                                             \
	{                                                                             \
		assert(node);                                                             \
		return (node->prev);                                                      \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief get data of a node.                                                 \
	 * @param node non-null pointer to a `ListNode_##unique_suffix`.              \
	 *                                                                            \
	 * @returns the data in the node.                                             \
	 */                                                                           \
	data_type ln_##unique_suffix##_get_data(                                      \
		const ListNode_##unique_suffix *const restrict node                       \
	)                                                                             \
	{                                                                             \
		assert(node);                                                             \
		return (node->data);                                                      \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief set the next pointer of a node.                                     \
	 * @param this_node non-null pointer to a `ListNode_##unique_suffix`.         \
	 * @param new_node pointer to insert.                                         \
	 *                                                                            \
	 * @returns the old pointer.                                                  \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_set_next(                      \
		ListNode_##unique_suffix *const restrict this_node,                       \
		ListNode_##unique_suffix *const restrict new_node                         \
	)                                                                             \
	{                                                                             \
		assert(this_node);                                                        \
		ListNode_##unique_suffix *const old_node = this_node->next;               \
                                                                                  \
		this_node->next = new_node;                                               \
		return (old_node);                                                        \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief set the previous pointer of a node.                                 \
	 * @param this_node non-null pointer to a `ListNode_##unique_suffix`.         \
	 * @param new_node pointer to insert.                                         \
	 *                                                                            \
	 * @returns the old pointer.                                                  \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_set_prev(                      \
		ListNode_##unique_suffix *const restrict this_node,                       \
		ListNode_##unique_suffix *const restrict new_node                         \
	)                                                                             \
	{                                                                             \
		assert(this_node);                                                        \
		ListNode_##unique_suffix *const old_node = this_node->prev;               \
                                                                                  \
		this_node->prev = new_node;                                               \
		return (old_node);                                                        \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief set the data of a node.                                             \
	 * @param this_node non-null pointer to a `ListNode_##unique_suffix`.         \
	 * @param new_data the data to swap in.                                       \
	 *                                                                            \
	 * @returns the old data.                                                     \
	 */                                                                           \
	data_type ln_##unique_suffix##_set_data(                                      \
		ListNode_##unique_suffix *const restrict this_node,                       \
		data_type new_data                                                        \
	)                                                                             \
	{                                                                             \
		assert(this_node);                                                        \
		data_type old_data = this_node->data;                                     \
                                                                                  \
		this_node->data = new_data;                                               \
		return (old_data);                                                        \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief allocate and initialise memory for a `ListNode_##unique_suffix`.    \
	 * @param data the data to be stored in the new node.                         \
	 * @param data_copy pointer to a function that can duplicate `data`.          \
	 *                                                                            \
	 * @returns pointer to the created node, NULL on failure.                     \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_new(                           \
		data_type data, duplicate_##unique_suffix *data_copy                      \
	)                                                                             \
	{                                                                             \
		ListNode_##unique_suffix *const restrict node =                           \
			xcalloc(1, sizeof(ListNode_##unique_suffix));                         \
                                                                                  \
		if (!node)                                                                \
			return (NULL);                                                        \
                                                                                  \
		if (data_copy)                                                            \
		{                                                                         \
			data_type cpy;                                                        \
                                                                                  \
			if (data_copy(&cpy, data) == false)                                   \
			{                                                                     \
				xfree(node);                                                      \
				return (NULL);                                                    \
			}                                                                     \
                                                                                  \
			data = cpy;                                                           \
		}                                                                         \
                                                                                  \
		ln_##unique_suffix##_set_data(node, data);                                \
		return (node);                                                            \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief unlink a node from a linked list.                                   \
	 * @param node pointer to a `ListNode_##unique_suffix`.                       \
	 *                                                                            \
	 * @returns pointer to the popped node.                                       \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_pop(                           \
		ListNode_##unique_suffix *const restrict node                             \
	)                                                                             \
	{                                                                             \
		if (!node)                                                                \
			return (NULL);                                                        \
                                                                                  \
		ListNode_##unique_suffix *const restrict next =                           \
			ln_##unique_suffix##_set_next(node, NULL);                            \
		ListNode_##unique_suffix *const restrict prev =                           \
			ln_##unique_suffix##_set_prev(node, NULL);                            \
                                                                                  \
		if (next)                                                                 \
			ln_##unique_suffix##_set_prev(next, prev);                            \
                                                                                  \
		if (prev)                                                                 \
			ln_##unique_suffix##_set_next(prev, next);                            \
                                                                                  \
		return (node);                                                            \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief pop and free a node from a linked list.                             \
	 * @param node pointer to a `ListNode_##unique_suffix`.                       \
	 *                                                                            \
	 * @returns the data in the node, 0 if `node` is NULL.                        \
	 */                                                                           \
	data_type ln_##unique_suffix##_del(                                           \
		ListNode_##unique_suffix *const restrict node                             \
	)                                                                             \
	{                                                                             \
		data_type zero;                                                           \
                                                                                  \
		memset(&zero, 0, sizeof(zero));                                           \
		if (!node)                                                                \
			return (zero);                                                        \
                                                                                  \
		data_type data = ln_##unique_suffix##_get_data(node);                     \
                                                                                  \
		xfree(ln_##unique_suffix##_pop(node));                                    \
		return (data);                                                            \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief insert a node after another node.                                   \
	 * @param this_node pointer to a node.                                        \
	 * @param other_node pointer to the node to insert.                           \
	 *                                                                            \
	 * @returns pointer to the newly inserted node, NULL if `other_node` is NULL. \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_insert_after(                  \
		ListNode_##unique_suffix *const restrict this_node,                       \
		ListNode_##unique_suffix *const restrict other_node                       \
	)                                                                             \
	{                                                                             \
		if (!this_node)                                                           \
			return (other_node);                                                  \
                                                                                  \
		if (!other_node)                                                          \
			return (NULL);                                                        \
                                                                                  \
		ListNode_##unique_suffix *const restrict this_next =                      \
			ln_##unique_suffix##_set_next(this_node, other_node);                 \
                                                                                  \
		if (this_next)                                                            \
			ln_##unique_suffix##_set_prev(this_next, other_node);                 \
                                                                                  \
		ln_##unique_suffix##_set_next(other_node, this_next);                     \
		ln_##unique_suffix##_set_prev(other_node, this_node);                     \
		return (other_node);                                                      \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief insert a node before another node.                                  \
	 * @param this_node pointer to a node.                                        \
	 * @param other_node pointer to the node to insert.                           \
	 *                                                                            \
	 * @returns pointer to the newly inserted node, NULL if `other_node` is NULL. \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_insert_before(                 \
		ListNode_##unique_suffix *const restrict this_node,                       \
		ListNode_##unique_suffix *const restrict other_node                       \
	)                                                                             \
	{                                                                             \
		if (!this_node)                                                           \
			return (other_node);                                                  \
                                                                                  \
		if (!other_node)                                                          \
			return (NULL);                                                        \
                                                                                  \
		ListNode_##unique_suffix *const restrict this_prev =                      \
			ln_##unique_suffix##_set_prev(this_node, other_node);                 \
                                                                                  \
		if (this_prev)                                                            \
			ln_##unique_suffix##_set_next(this_prev, other_node);                 \
                                                                                  \
		ln_##unique_suffix##_set_next(other_node, this_node);                     \
		ln_##unique_suffix##_set_prev(other_node, this_prev);                     \
		return (other_node);                                                      \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief swap data of two nodes.                                             \
	 * @param this_node pointer to the node to be swapped out.                    \
	 * @param other_node pointer to the node to be swapped in.                    \
	 *                                                                            \
	 * @returns pointer to the swapped in node, NULL if `other_node` is NULL.     \
	 */                                                                           \
	ListNode_##unique_suffix *ln_##unique_suffix##_swap(                          \
		ListNode_##unique_suffix *const restrict this_node,                       \
		ListNode_##unique_suffix *const restrict other_node                       \
	)                                                                             \
	{                                                                             \
		if (!this_node)                                                           \
			return (other_node);                                                  \
                                                                                  \
		if (!other_node)                                                          \
			return (NULL);                                                        \
                                                                                  \
		ln_##unique_suffix##_set_data(                                            \
			other_node,                                                           \
			ln_##unique_suffix##_set_data(                                        \
				this_node, ln_##unique_suffix##_get_data(other_node)              \
			)                                                                     \
		);                                                                        \
		return (other_node);                                                      \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief free a linked list.                                                 \
	 * @param head pointer to the start of the linked list.                       \
	 * @param data_free function that will be called to free data in the nodes.   \
	 *                                                                            \
	 * @returns NULL always.                                                      \
	 */                                                                           \
	void *list_##unique_suffix##_del(                                             \
		ListNode_##unique_suffix *const head,                                     \
		free_mem_##unique_suffix *data_free                                       \
	)                                                                             \
	{                                                                             \
		if (!head)                                                                \
			return (NULL);                                                        \
                                                                                  \
		ListNode_##unique_suffix *walk = head;                                    \
                                                                                  \
		while (walk)                                                              \
		{                                                                         \
			ListNode_##unique_suffix *next =                                      \
				ln_##unique_suffix##_get_next(walk);                              \
			data_type data = ln_##unique_suffix##_del(walk);                      \
                                                                                  \
			if (data_free)                                                        \
				data_free(data);                                                  \
                                                                                  \
			walk = next;                                                          \
		}                                                                         \
                                                                                  \
		return (NULL);                                                            \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief stringify a linked list.                                            \
	 * @param head pointer to the start of the linked list to stringify.          \
	 * @param data_tostr pointer to a function that formats data in the nodes     \
	 * into a string.                                                             \
	 *                                                                            \
	 * @returns pointer to the stringified linked list, NULL on error.            \
	 */                                                                           \
	char *list_##unique_suffix##_tostr(                                           \
		ListNode_##unique_suffix const *const restrict head,                      \
		stringify_data_##unique_suffix *data_tostr                                \
	)                                                                             \
	{                                                                             \
		if (!head || !data_tostr)                                                 \
			return (NULL);                                                        \
                                                                                  \
		const char link[] = " <--> ";                                             \
		char *restrict list_str =                                                 \
			data_tostr(ln_##unique_suffix##_get_data(head));                      \
                                                                                  \
		if (!list_str)                                                            \
			return (NULL);                                                        \
                                                                                  \
		intmax_t list_str_len = strlen(list_str);                                 \
		const ListNode_##unique_suffix *restrict walk =                           \
			ln_##unique_suffix##_get_next(head);                                  \
                                                                                  \
		while (list_str && walk)                                                  \
		{                                                                         \
			char *restrict data_str =                                             \
				data_tostr(ln_##unique_suffix##_get_data(walk));                  \
                                                                                  \
			if (!data_str)                                                        \
				break;                                                            \
                                                                                  \
			list_str_len += sizeof(link) - 1 + strlen(data_str);                  \
			list_str = xrealloc(list_str, list_str_len + 1);                      \
			if (list_str)                                                         \
				strcat(strcat(list_str, link), data_str);                         \
                                                                                  \
			xfree(data_str);                                                      \
			walk = ln_##unique_suffix##_get_next(walk);                           \
		}                                                                         \
                                                                                  \
		if (walk)                                                                 \
		{                                                                         \
			xfree(list_str);                                                      \
			list_str = NULL;                                                      \
		}                                                                         \
                                                                                  \
		return (list_str);                                                        \
	}                                                                             \
                                                                                  \
	/**                                                                           \
	 * @brief stringify a linked list in reverse.                                 \
	 * @param tail pointer to the tail of the linked list.                        \
	 * @param data_tostr pointer to a function that formats data in the nodes     \
	 * into a string.                                                             \
	 *                                                                            \
	 * @returns pointer to the stringified linked list, NULL on error.            \
	 */                                                                           \
	char *list_##unique_suffix##_tostr_reversed(                                  \
		ListNode_##unique_suffix const *const restrict tail,                      \
		stringify_data_##unique_suffix *data_tostr                                \
	)                                                                             \
	{                                                                             \
		if (!tail || !data_tostr)                                                 \
			return (NULL);                                                        \
                                                                                  \
		const char link[] = " <--> ";                                             \
		char *restrict list_str =                                                 \
			data_tostr(ln_##unique_suffix##_get_data(tail));                      \
                                                                                  \
		if (!list_str)                                                            \
			return (NULL);                                                        \
                                                                                  \
		intmax_t list_str_len = strlen(list_str);                                 \
		const ListNode_##unique_suffix *restrict walk =                           \
			ln_##unique_suffix##_get_prev(tail);                                  \
                                                                                  \
		while (list_str && walk)                                                  \
		{                                                                         \
			data_type d = ln_##unique_suffix##_get_data(walk);                    \
			char *restrict data_str = data_tostr(d);                              \
                                                                                  \
			if (!data_str)                                                        \
				break;                                                            \
                                                                                  \
			list_str_len += sizeof(link) - 1 + strlen(data_str);                  \
			list_str = xrealloc(list_str, list_str_len + 1);                      \
			if (list_str)                                                         \
				strcat(strcat(list_str, link), data_str);                         \
                                                                                  \
			xfree(data_str);                                                      \
			walk = ln_##unique_suffix##_get_prev(walk);                           \
		}                                                                         \
                                                                                  \
		if (walk)                                                                 \
		{                                                                         \
			xfree(list_str);                                                      \
			list_str = NULL;                                                      \
		}                                                                         \
                                                                                  \
		return (list_str);                                                        \
	}

#endif /* LISTNODE_METHODS_MACROS_H */
