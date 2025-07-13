#ifndef DS_DEQUE_METHODS_MACROS_H
#define DS_DEQUE_METHODS_MACROS_H

#include <string.h> /* strcpy */

#include "ListNode_methods_macros.h"

/*!
 * @brief define methods for a Deque type.
 * @param unique_suffix a unique suffix for method names and Deque type.
 * @param data_type the type of objects the nodes store.
 */
#define DEFINE_DEQUE_METHODS(unique_suffix, data_type)                                       \
	DEFINE_LISTNODE_METHODS(unique_suffix, data_type)                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief allocate and initialise memory for a deque.                                    \
	 *                                                                                       \
	 * @returns pointer to the deque, NULL on failure.                                       \
	 */                                                                                      \
	Deque_##unique_suffix *dq_##unique_suffix##_new(void)                                    \
	{                                                                                        \
		return (xcalloc(1, sizeof(Deque_##unique_suffix)));                                  \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief free all the nodes of a deque.                                                 \
	 * @param dq the deque to operate on.                                                    \
	 * @param data_free pointer to a function that can free the data in the nodes.           \
	 */                                                                                      \
	void dq_##unique_suffix##_clear(                                                         \
		Deque_##unique_suffix *const restrict dq,                                            \
		free_mem_##unique_suffix *data_free                                                  \
	)                                                                                        \
	{                                                                                        \
		if (!dq || !dq->head)                                                                \
			return;                                                                          \
                                                                                             \
		ListNode_##unique_suffix *next_node =                                                \
			ln_##unique_suffix##_get_next(dq->head);                                         \
                                                                                             \
		while (dq->head)                                                                     \
		{                                                                                    \
			data_type d = ln_##unique_suffix##_del(dq->head);                                \
                                                                                             \
			if (data_free)                                                                   \
				data_free(d);                                                                \
                                                                                             \
			dq->head = next_node;                                                            \
			next_node =                                                                      \
				dq->head ? ln_##unique_suffix##_get_next(dq->head) : NULL;                   \
		}                                                                                    \
                                                                                             \
		dq->head = NULL;                                                                     \
		dq->tail = NULL;                                                                     \
		dq->len = 0;                                                                         \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief free memory of a deque.                                                        \
	 * @param dq pointer to the deque to free.                                               \
	 * @param data_free pointer to a function that can free data in the nodes.               \
	 *                                                                                       \
	 * @returns NULL always.                                                                 \
	 */                                                                                      \
	void *dq_##unique_suffix##_del(                                                          \
		Deque_##unique_suffix *const restrict dq,                                            \
		free_mem_##unique_suffix *data_free                                                  \
	)                                                                                        \
	{                                                                                        \
		dq_##unique_suffix##_clear(dq, data_free);                                           \
		xfree(dq);                                                                           \
		return (NULL);                                                                       \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief add a new node to the head of the deque.                                       \
	 * @param dq the deque to operate on.                                                    \
	 * @param data data that the new node will hold.                                         \
	 * @param data_dup function that can duplicate `data`.                                   \
	 *                                                                                       \
	 * @returns pointer to the new node, NULL on failure.                                    \
	 */                                                                                      \
	ListNode_##unique_suffix *dq_##unique_suffix##_push_head(                                \
		Deque_##unique_suffix *const restrict dq, data_type data,                            \
		duplicate_##unique_suffix *data_dup                                                  \
	)                                                                                        \
	{                                                                                        \
		if (!dq)                                                                             \
			return (NULL);                                                                   \
                                                                                             \
		ListNode_##unique_suffix *const restrict nw =                                        \
			ln_##unique_suffix##_new(data, data_dup);                                        \
                                                                                             \
		if (!nw)                                                                             \
			return (NULL);                                                                   \
                                                                                             \
		dq->head = ln_##unique_suffix##_insert_before(dq->head, nw);                         \
		if (!dq->tail)                                                                       \
			dq->tail = nw;                                                                   \
                                                                                             \
		++(dq->len);                                                                         \
		return (nw);                                                                         \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief add a node to the tail of a deque.                                             \
	 * @param dq the deque to operate on.                                                    \
	 * @param data data that the node will hold.                                             \
	 * @param data_dup function that can duplicate data,                                     \
	 * if NULL a simple assignment of the data is done.                                      \
	 *                                                                                       \
	 * @returns pointer to the newly added node, NULL on error.                              \
	 */                                                                                      \
	ListNode_##unique_suffix *dq_##unique_suffix##_push_tail(                                \
		Deque_##unique_suffix *const restrict dq, data_type data,                            \
		duplicate_##unique_suffix *data_dup                                                  \
	)                                                                                        \
	{                                                                                        \
		if (!dq)                                                                             \
			return (NULL);                                                                   \
                                                                                             \
		ListNode_##unique_suffix *const restrict nw =                                        \
			ln_##unique_suffix##_new(data, data_dup);                                        \
		if (!nw)                                                                             \
			return (NULL);                                                                   \
                                                                                             \
		dq->tail = ln_##unique_suffix##_insert_after(dq->tail, nw);                          \
		if (!dq->head)                                                                       \
			dq->head = nw;                                                                   \
                                                                                             \
		++(dq->len);                                                                         \
		return (nw);                                                                         \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief pop a node from the head of a deque.                                           \
	 * @param dq the deque to operate on.                                                    \
	 *                                                                                       \
	 * @returns the data of the popped node, 0 if dq or head is NULL.                        \
	 */                                                                                      \
	data_type dq_##unique_suffix##_pop_head(                                                 \
		Deque_##unique_suffix *const restrict dq                                             \
	)                                                                                        \
	{                                                                                        \
		data_type zero;                                                                      \
                                                                                             \
		memset(&zero, 0, sizeof(zero));                                                      \
		if (!dq || !dq->head)                                                                \
			return (zero);                                                                   \
                                                                                             \
		ListNode_##unique_suffix *const node = dq->head;                                     \
                                                                                             \
		dq->head = ln_##unique_suffix##_get_next(node);                                      \
		data_type d = ln_##unique_suffix##_del(node);                                        \
                                                                                             \
		if (!dq->head)                                                                       \
			dq->tail = NULL;                                                                 \
                                                                                             \
		if (dq->len > 0)                                                                     \
			--(dq->len);                                                                     \
                                                                                             \
		return (d);                                                                          \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief pop a node from the tail of a deque.                                           \
	 * @param dq the deque to operate on.                                                    \
	 *                                                                                       \
	 * @returns the data of the popped node, 0 if dq or head is NULL.                        \
	 */                                                                                      \
	data_type dq_##unique_suffix##_pop_tail(                                                 \
		Deque_##unique_suffix *const restrict dq                                             \
	)                                                                                        \
	{                                                                                        \
		data_type zero;                                                                      \
                                                                                             \
		memset(&zero, 0, sizeof(zero));                                                      \
		if (!dq || !dq->tail)                                                                \
			return (zero);                                                                   \
                                                                                             \
		ListNode_##unique_suffix *node = dq->tail;                                           \
                                                                                             \
		dq->tail = ln_##unique_suffix##_get_prev(node);                                      \
		data_type d = ln_##unique_suffix##_del(node);                                        \
                                                                                             \
		if (!dq->tail)                                                                       \
			dq->head = NULL;                                                                 \
                                                                                             \
		if (dq->len > 0)                                                                     \
			--(dq->len);                                                                     \
                                                                                             \
		return (d);                                                                          \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief create a new deque from an array of objects.                                   \
	 * @param array pointer to the array of objects.                                         \
	 * @param len number of items in the array.                                              \
	 * @param data_dup function that can duplicate the objects.                              \
	 * @param data_free function that delete the delete objects,                             \
	 * should always be specified if `data_dup` is specified.                                \
	 *                                                                                       \
	 * @returns pointer to the new deque, NULL on error.                                     \
	 */                                                                                      \
	Deque_##unique_suffix *dq_##unique_suffix##_from_array(                                  \
		data_type *const restrict array, const intmax_t len,                                 \
		duplicate_##unique_suffix *data_dup,                                                 \
		free_mem_##unique_suffix *data_free                                                  \
	)                                                                                        \
	{                                                                                        \
		if (!array || len < 1)                                                               \
			return (NULL);                                                                   \
                                                                                             \
		/* A deletion function is required so as to free all allocated memory on failure. */ \
		if (data_dup && !data_free)                                                          \
			return (NULL);                                                                   \
                                                                                             \
		Deque_##unique_suffix *restrict new_q = dq_##unique_suffix##_new();                  \
                                                                                             \
		if (!new_q)                                                                          \
			return (NULL);                                                                   \
                                                                                             \
		for (intmax_t i = 0; i < len; ++i)                                                   \
		{                                                                                    \
			data_type data = array[i];                                                       \
                                                                                             \
			if (!dq_##unique_suffix##_push_tail(new_q, data, data_dup))                      \
			{                                                                                \
				new_q = dq_##unique_suffix##_del(new_q, data_free);                          \
				break;                                                                       \
			}                                                                                \
		}                                                                                    \
                                                                                             \
		return (new_q);                                                                      \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief create an array from a deque.                                                  \
	 * @param dq the deque.                                                                  \
	 * @param data_dup function that can duplicate the objects.                              \
	 * @param data_free function that delete the delete objects,                             \
	 * should always be specified if `data_dup` is specified.                                \
	 *                                                                                       \
	 * @returns pointer to the data array on success, NULL on failure.                       \
	 */                                                                                      \
	data_type *dq_##unique_suffix##_to_array(                                                \
		const Deque_##unique_suffix *const restrict dq,                                      \
		duplicate_##unique_suffix *data_dup,                                                 \
		free_mem_##unique_suffix *data_free                                                  \
	)                                                                                        \
	{                                                                                        \
		if (!dq || !dq->head || dq->len < 1)                                                 \
			return (NULL);                                                                   \
                                                                                             \
		/* A deletion function is required so as to free all allocated memory on failure. */ \
		if (data_dup && !data_free)                                                          \
			return (NULL);                                                                   \
                                                                                             \
		data_type *const restrict array =                                                    \
			xmalloc(sizeof(*array) * (dq->len + 1));                                         \
                                                                                             \
		if (!array)                                                                          \
			return (NULL);                                                                   \
                                                                                             \
		ListNode_##unique_suffix *node = dq->head;                                           \
		for (size_t i = 0; node;                                                             \
			 node = ln_##unique_suffix##_get_next(node), ++i)                                \
		{                                                                                    \
			data_type data = ln_##unique_suffix##_get_data(node);                            \
                                                                                             \
			array[i] = data;                                                                 \
			if (data_dup)                                                                    \
			{                                                                                \
				if (data_dup(&array[i], data) == false)                                      \
				{                                                                            \
					for (size_t k = 0; k < i; k++)                                           \
						data_free(array[k]);                                                 \
                                                                                             \
					xfree(array);                                                            \
					return (NULL);                                                           \
				}                                                                            \
			}                                                                                \
		}                                                                                    \
                                                                                             \
		memset(&array[dq->len], 0, sizeof(*array));                                          \
		return (array);                                                                      \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief stringify a deque.                                                             \
	 * @param dq the deque to print.                                                         \
	 * @param data_tostr function that will be called to print data in nodes.                \
	 *                                                                                       \
	 * @returns number of bytes printed, negative number on error.                           \
	 */                                                                                      \
	char *dq_##unique_suffix##_tostr(                                                        \
		const Deque_##unique_suffix *const restrict dq,                                      \
		stringify_data_##unique_suffix *data_tostr                                           \
	)                                                                                        \
	{                                                                                        \
		if (!dq)                                                                             \
			return (NULL);                                                                   \
                                                                                             \
		if (!dq->head)                                                                       \
		{                                                                                    \
			char *const restrict s = xmalloc(sizeof("(NULL)"));                              \
                                                                                             \
			if (s)                                                                           \
				strcpy(s, "(NULL)");                                                         \
                                                                                             \
			return (s);                                                                      \
		}                                                                                    \
                                                                                             \
		return (list_##unique_suffix##_tostr(dq->head, data_tostr));                         \
	}                                                                                        \
                                                                                             \
	/**                                                                                      \
	 * @brief stringify a deque from tail to head.                                           \
	 * @param dq the deque to print.                                                         \
	 * @param data_tostr function that will be called to print data in nodes.                \
	 *                                                                                       \
	 * @returns number of bytes printed, negative number on error.                           \
	 */                                                                                      \
	char *dq_##unique_suffix##_tostr_reversed(                                               \
		const Deque_##unique_suffix *const restrict dq,                                      \
		stringify_data_##unique_suffix *data_tostr                                           \
	)                                                                                        \
	{                                                                                        \
		if (!dq)                                                                             \
			return (NULL);                                                                   \
                                                                                             \
		if (!dq->tail)                                                                       \
		{                                                                                    \
			char *const restrict s = xmalloc(sizeof("(NULL)"));                              \
                                                                                             \
			if (s)                                                                           \
				strcpy(s, "(NULL)");                                                         \
                                                                                             \
			return (s);                                                                      \
		}                                                                                    \
                                                                                             \
		return (list_##unique_suffix##_tostr_reversed(dq->tail, data_tostr));                \
	}

#endif /* DS_DEQUE_METHODS_MACROS_H */
