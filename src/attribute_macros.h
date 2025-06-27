#ifndef ATTRIBUTE_MACROS_H
#define ATTRIBUTE_MACROS_H

#if defined(__has_attribute)
	/*! https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005fattribute.html */
	#define _has_attribute(attribute_name) __has_attribute(attribute_name)
#else
	#define _has_attribute(attribute_name)
#endif  // defined(__has_attribute)

#if defined(__has_include)
	/*! https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html */
	#define _has_include(header) __has_include(header)
#else
	#define _has_include(header)
#endif  // defined(__has_include)

#if _has_include(<stdalign.h>)
	#include <stdalign.h>  // alignas
	#define _alignas(alignment) alignas(alignment)
	#define _alignof(T) alignof(T)
#elif __STDC_VERSION__ >= 202311L
	#define _alignas(alignment) alignas(alignment)
	#define _alignof(T) alignof(T)
#else
	#if _has_attribute(aligned)
		#define _alignas(alignment) __attribute__((aligned(alignment)))
	#else
		#define _alignas(alignment)
	#endif  // _has_attribute(aligned)
	#define _alignof(T) sizeof(T)
#endif  // _has_include(<stdalign.h>)

/* https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html */

#if _has_attribute(counted_by)
	/**
	 * @brief may be attached to the C99 flexible array member of a structure.
	 *
	 * It indicates that the number of the elements of the array is given by
	 * the field `counter` in the same structure as the flexible array member.
	 *
	 * @counted_by: name of the struct member that gives the size of the FAM.
	 */
	#define _counted_by(counter) __attribute__((counted_by(counter)))
#else
	#define _counted_by(counter)
#endif  // _has_attribute(counted_by)

/* https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html */

#if _has_attribute(alloc_size)
	/**
	 * @brief indicates that a function returns a pointer to a memory block of
	 * size `position` or `position-1 * position-2` bytes.
	 *
	 * `alloc_size(position)`
	 * `alloc_size(position-1, position-2)`
	 */
	#define _alloc_size(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
	#define _alloc_size(...)
#endif  // _has_attribute(alloc_size)

#if _has_attribute(malloc)
	/*!
	 * @brief indicates that a function is malloc-like.
	 *
	 * _malloc_free(deallocator[, ptr_position])
	 *
	 * A malloc-like function returns a pointer P that cannot alias any
	 * other pointer valid when the function returns, and moreover no
	 * pointers to valid objects occur in any storage addressed by P.
	 */
	#define _malloc __attribute__((malloc))
	#if defined __GNUC__ && !__clang__
		/**
		 * @brief same as malloc.
		 *
		 * _malloc_free(deallocator[, ptr_position])
		 *
		 * @deallocator: a suitable deallocation function for pointers returned
		 * from the malloc-like function.
		 * @ptr_position: parameter position, counting from 1, of the pointer
		 * to be deallocated in the deallocating function.
		 */
		#define _malloc_free(...) __attribute__((malloc(__VA_ARGS__)))
	#else
		#define _malloc_free(...)
	#endif  // defined __GNUC__ && !__clang__
#else
	#define _malloc
	#define _malloc_free(...)
#endif  // _has_attribute(malloc)

#if _has_attribute(nonnull)
	/**
	 * @brief indicates that a function only accepts non-null pointers.
	 */
	#define _nonnull __attribute__((nonnull))
	/*!
	 * @brief same as _nonnull.
	 *
	 * `_nonnull_pos(arg_position, …)`
	 *
	 * @arg_position: list of parameter positions of non-null pointers
	 * counting from 1.
	 */
	#define _nonnull_pos(...) __attribute__((nonnull(__VA_ARGS__)))
#else
	#define _nonnull
	#define _nonnull_pos(...)
#endif  // _has_attribute(nonnull)

#if _has_attribute(format)
	/**
	 * @brief specifies that a function takes printf/scanf/strftime/strfmon style arguments.
	 *
	 * @archetype: determines how the format string is interpreted.
	 * Valid archetypes include printf, scanf, strftime or strfmon.
	 * @string_position: parameter position, counting from 1, of the format string.
	 * @va_position: parameter position of the first variadic argument.
	 */
	#define _format(archetype, string_position, va_position)                  \
		__attribute__((format(archetype, string_position, va_position)))
#else
	#define _format(archetype, string_position, va_position)
#endif  // _has_attribute(format)

#if _has_attribute(assume_aligned)
	/*!
	 * @brief indicates pointer returned by function is aligned on the given
	 * boundary.
	 *
	 * assume_aligned(alignment[, offset])
	 *
	 * @alignment: a power of 2 greater than 1.
	 * @offset: a number less than alignment indicating offset from alignment.
	 */
	#define _assume_aligned(...) __attribute__((assume_aligned(__VA_ARGS__)))
#else
	#define _assume_aligned(...)
#endif

#endif  // ATTRIBUTE_MACROS_H
