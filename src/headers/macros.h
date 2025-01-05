#ifndef INFIX_PROJ_MACROS_H
#define INFIX_PROJ_MACROS_H

/* INFO: 1 based indexing. */
#define ATTR_ALLOC_SIZE(...)
#define ATTR_COUNTED_BY(counter)
#define ATTR_MALLOC
#define ATTR_MALLOC_FREE(...)
#define ATTR_NONNULL
/* INFO: 1 based indexing. */
#define ATTR_NONNULL_IDX(...)

#if defined __has_attribute

	/* https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html */
	#if __has_attribute(alloc_size)
		#undef ATTR_ALLOC_SIZE
		#define ATTR_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
	#endif /*__has_attribute(alloc_size)*/

	#if __has_attribute(counted_by)
		#undef ATTR_COUNTED_BY
		#define ATTR_COUNTED_BY(counter) __attribute__((counted_by(counter)))
	#endif /* __has_attribute(counted_by) */

	/* https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html */
	#if __has_attribute(malloc)
		#undef ATTR_MALLOC
		#define ATTR_MALLOC __attribute__((malloc))

		#if defined __GNUC__ && !__clang__
			#undef ATTR_MALLOC_FREE
			#define ATTR_MALLOC_FREE(...) __attribute__((malloc(__VA_ARGS__)))
		#endif /* defined __GNUC__ && !__clang__ */
	#endif     /*__has_attribute(malloc)*/

	#if __has_attribute(nonnull)
		#undef ATTR_NONNULL
		#define ATTR_NONNULL __attribute__((nonnull))
		#undef ATTR_NONNULL_IDX
		#define ATTR_NONNULL_IDX(...) __attribute__((nonnull(__VA_ARGS__)))
	#endif /*__has_attribute(nonnull)*/

#endif /* defined __has_attribute */

/* program_invocation_name */
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#endif /* INFIX_PROJ_MACROS_H */
