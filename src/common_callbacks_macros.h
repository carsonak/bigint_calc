#ifndef COMMON_CALLBACKS_MACROS_H
#define COMMON_CALLBACKS_MACROS_H

#include <stdbool.h> /* bool */

#define DECLARE_COMMON_CALLBACKS(unique_suffix, data_type)                    \
	/**                                                                       \
	 * @brief a function that duplicates an object.                           \
	 *                                                                        \
	 * @param dest address to store the duplicated data.                      \
	 * @param data the object to be duplicated.                               \
	 * @returns true on success, false on failure.                            \
	 */                                                                       \
	typedef bool(duplicate_##unique_suffix)(                                  \
		data_type *const restrict dest, const data_type data                  \
	);                                                                        \
                                                                              \
	/**                                                                       \
	 * @brief a function that frees an object from memory.                    \
	 *                                                                        \
	 * @param data the object to delete.                                      \
	 */                                                                       \
	typedef void(free_mem_##unique_suffix)(data_type data);                   \
                                                                              \
	/**                                                                       \
	 * @brief a function that turns an object into a string.                  \
	 *                                                                        \
	 * @param data the object to stringify.                                   \
	 * @returns pointer to the string, NULL error.                            \
	 */                                                                       \
	typedef char *(stringify_data_##unique_suffix)(const data_type data);

#endif /* COMMON_CALLBACKS_MACROS_H */
