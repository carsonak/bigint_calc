#ifndef DEQUE_UNIQUE_SUFFIX
	#error "Missing definition for `DEQUE_UNIQUE_SUFFIX`."
#endif

#ifndef DEQUE_DATA_TYPE
	#error "Missing definition for `DEQUE_DATA_TYPE`."
#endif

#define LISTNODE_UNIQUE_SUFFIX DEQUE_UNIQUE_SUFFIX
#define LISTNODE_DATA_TYPE DEQUE_DATA_TYPE
#include "ListNode_struct_def.h"

#include "len_type.h"

#define DQ_CONCAT_PLAIN(tok0, tok1) tok0##tok1
#define DQ_CONCAT(tok0, tok1) DQ_CONCAT_PLAIN(tok0, tok1)

#define DEQUE_STRUCT_TAG DQ_CONCAT(Deque_, DEQUE_UNIQUE_SUFFIX)

#define LISTNODE_STRUCT_TAG DQ_CONCAT(ListNode_, DEQUE_UNIQUE_SUFFIX)

/*!
 * @brief a `deque` type.
 */
struct DEQUE_STRUCT_TAG
{
	/*! @public number of nodes in the deque. */
	len_ty len;
	/*! @public pointer to the head node of the deque. */
	struct LISTNODE_STRUCT_TAG *head;
	/*! @public pointer to the tail node of the deque. */
	struct LISTNODE_STRUCT_TAG *tail;
};

#undef LISTNODE_STRUCT_TAG

#undef DEQUE_UNIQUE_SUFFIX
#undef DEQUE_DATA_TYPE

#undef DQ_CONCAT_PLAIN
#undef DQ_CONCAT

#undef DEQUE_STRUCT_TAG
