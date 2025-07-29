#ifndef LISTNODE_UNIQUE_SUFFIX
	#error "Missing definition for `LISTNODE_UNIQUE_SUFFIX`."
#endif

#ifndef LISTNODE_DATA_TYPE
	#error "Missing definition for `LISTNODE_DATA_TYPE`."
#endif

#define LN_CONCAT_PLAIN(tok0, tok1) tok0##tok1
#define LN_CONCAT(tok0, tok1) LN_CONCAT_PLAIN(tok0, tok1)

#define LISTNODE_STRUCT_TAG LN_CONCAT(ListNode_, LISTNODE_UNIQUE_SUFFIX)

/*!
 * @brief a doubly linked node.
 */
struct LISTNODE_STRUCT_TAG
{
	/*! @public pointer to the next node. */
	struct LISTNODE_STRUCT_TAG *restrict next;
	/*! @public pointer to the previous node. */
	struct LISTNODE_STRUCT_TAG *restrict prev;
	/*! @public pointer to the data for the node. */
	LISTNODE_DATA_TYPE data;
};

#undef LISTNODE_UNIQUE_SUFFIX
#undef LISTNODE_DATA_TYPE

#undef LN_CONCAT_PLAIN
#undef LN_CONCAT

#undef LISTNODE_STRUCT_TAG
