#ifndef LISTNODE_UNIQUE_SUFFIX
	#error "Missing definition for `LISTNODE_UNIQUE_SUFFIX`."
#endif

#ifndef LISTNODE_DATA_TYPE
	#error "Missing definition for `LISTNODE_DATA_TYPE`."
#endif

#define COMMON_CALLBACKS_UNIQUE_SUFFIX LISTNODE_UNIQUE_SUFFIX
#define COMMON_CALLBACKS_DATA_TYPE LISTNODE_DATA_TYPE
#include "common_callback_types.h"

#include "compiler_attributes_macros.h"

#define LN_CONCAT_PLAIN(tok0, tok1) tok0##tok1
#define LN_CONCAT(tok0, tok1) LN_CONCAT_PLAIN(tok0, tok1)

#define LISTNODE_STRUCT_TAG LN_CONCAT(ListNode_, LISTNODE_UNIQUE_SUFFIX)

#define LISTNODE_METHODNAME(name)                                             \
	LN_CONCAT(LN_CONCAT(ln_, LISTNODE_UNIQUE_SUFFIX), LN_CONCAT(_, name))

#define LIST_METHODNAME(name)                                                 \
	LN_CONCAT(LN_CONCAT(list_, LISTNODE_UNIQUE_SUFFIX), LN_CONCAT(_, name))

typedef struct LISTNODE_STRUCT_TAG LISTNODE_STRUCT_TAG;

/* alloc and free */

LISTNODE_STRUCT_TAG *
	LISTNODE_METHODNAME(pop)(LISTNODE_STRUCT_TAG *const restrict node);
LISTNODE_DATA_TYPE
LISTNODE_METHODNAME(del)(LISTNODE_STRUCT_TAG *const restrict node);
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(new)(
	LISTNODE_DATA_TYPE data,
	LN_CONCAT(duplicate_, LISTNODE_UNIQUE_SUFFIX) * copy_data
) _malloc _malloc_free(LISTNODE_METHODNAME(del));

void *LIST_METHODNAME(del)(
	LISTNODE_STRUCT_TAG *const head,
	LN_CONCAT(free_mem_, LISTNODE_UNIQUE_SUFFIX) * free_data
);

/* access */

LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(get_next)(
	const LISTNODE_STRUCT_TAG *const restrict node
) _nonnull;
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(get_prev)(
	const LISTNODE_STRUCT_TAG *const restrict node
) _nonnull;
LISTNODE_DATA_TYPE LISTNODE_METHODNAME(get_data)(
	const LISTNODE_STRUCT_TAG *const restrict node
) _nonnull;

/* manipulate */

LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(set_next)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
) _nonnull_pos(1);
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(set_prev)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
) _nonnull_pos(1);
LISTNODE_DATA_TYPE LISTNODE_METHODNAME(set_data)(
	LISTNODE_STRUCT_TAG *const restrict node, LISTNODE_DATA_TYPE data
);

LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(insert_after)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
);
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(insert_before)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
);
LISTNODE_STRUCT_TAG *LISTNODE_METHODNAME(swap)(
	LISTNODE_STRUCT_TAG *const restrict this_node,
	LISTNODE_STRUCT_TAG *const restrict other_node
);

/* print */

char *LIST_METHODNAME(tostr)(
	LISTNODE_STRUCT_TAG const *const restrict head,
	LN_CONCAT(stringify_data_, LISTNODE_UNIQUE_SUFFIX) * data_tostr
) _malloc;
char *LIST_METHODNAME(tostr_reversed)(
	LISTNODE_STRUCT_TAG const *const restrict tail,
	LN_CONCAT(stringify_data_, LISTNODE_UNIQUE_SUFFIX) * data_tostr
) _malloc;

#undef LISTNODE_UNIQUE_SUFFIX
#undef LISTNODE_DATA_TYPE

#undef LN_CONCAT_PLAIN
#undef LN_CONCAT

#undef LISTNODE_STRUCT_TAG
#undef LISTNODE_METHODNAME
#undef LIST_METHODNAME
