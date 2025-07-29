#ifndef DEQUE_UNIQUE_SUFFIX
	#error "Missing definition for `DEQUE_UNIQUE_SUFFIX`."
#endif

#ifndef DEQUE_DATA_TYPE
	#error "Missing definition for `DEQUE_DATA_TYPE`."
#endif

#define LISTNODE_UNIQUE_SUFFIX DEQUE_UNIQUE_SUFFIX
#define LISTNODE_DATA_TYPE DEQUE_DATA_TYPE
#include "ListNode_prototypes.h"

#include "compiler_attributes_macros.h"
#include "len_type.h"

#define DQ_CONCAT_PLAIN(tok0, tok1) tok0##tok1
#define DQ_CONCAT(tok0, tok1) DQ_CONCAT_PLAIN(tok0, tok1)

#define DEQUE_STRUCT_TAG DQ_CONCAT(Deque_, DEQUE_UNIQUE_SUFFIX)

#define DEQUE_METHODNAME(name)                                                \
	DQ_CONCAT(DQ_CONCAT(dq_, DEQUE_UNIQUE_SUFFIX), DQ_CONCAT(_, name))

#define LISTNODE_STRUCT_TAG DQ_CONCAT(ListNode_, DEQUE_UNIQUE_SUFFIX)

typedef struct DEQUE_STRUCT_TAG DEQUE_STRUCT_TAG;

/* alloc and free */

void *DEQUE_METHODNAME(del)(
	DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * free_data
);
DEQUE_STRUCT_TAG *
	DEQUE_METHODNAME(new)(void) _malloc _malloc_free(DEQUE_METHODNAME(del));

/* manipulate */

LISTNODE_STRUCT_TAG *DEQUE_METHODNAME(push_head)(
	DEQUE_STRUCT_TAG *const restrict dq, DEQUE_DATA_TYPE data,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * copy_data
);
LISTNODE_STRUCT_TAG *DEQUE_METHODNAME(push_tail)(
	DEQUE_STRUCT_TAG *const restrict dq, DEQUE_DATA_TYPE data,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * copy_data
);
DEQUE_DATA_TYPE
DEQUE_METHODNAME(pop_head)(DEQUE_STRUCT_TAG *const restrict dq);
DEQUE_DATA_TYPE
DEQUE_METHODNAME(pop_tail)(DEQUE_STRUCT_TAG *const restrict dq);
void DEQUE_METHODNAME(clear)(
	DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * free_data
);

/* array conversion */

DEQUE_STRUCT_TAG *DEQUE_METHODNAME(from_array)(
	DEQUE_DATA_TYPE *const restrict array, const len_ty len,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * copy_data,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * delete_data
);
DEQUE_DATA_TYPE *DEQUE_METHODNAME(to_array)(
	const DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(duplicate_, DEQUE_UNIQUE_SUFFIX) * copy_data,
	DQ_CONCAT(free_mem_, DEQUE_UNIQUE_SUFFIX) * free_data
);

/* print */

char *DEQUE_METHODNAME(tostr)(
	DEQUE_STRUCT_TAG const *const restrict dq,
	DQ_CONCAT(stringify_data_, DEQUE_UNIQUE_SUFFIX) * print_data
);
char *DEQUE_METHODNAME(tostr_reversed)(
	const DEQUE_STRUCT_TAG *const restrict dq,
	DQ_CONCAT(stringify_data_, DEQUE_UNIQUE_SUFFIX) * print_data
);

#undef LISTNODE_STRUCT_TAG

#undef DEQUE_UNIQUE_SUFFIX
#undef DEQUE_DATA_TYPE

#undef DQ_CONCAT_PLAIN
#undef DQ_CONCAT

#undef DEQUE_STRUCT_TAG
#undef DEQUE_METHODNAME
