#ifndef BIGINT_LIST_TYPE_TYPEDEFS_H
#define BIGINT_LIST_TYPE_TYPEDEFS_H

/*!
 * @brief a function that duplicates an object.
 *
 * @param[in] data pointer to the object to be duplicated.
 *
 * @return pointer to the duplicated object, NULL on failure.
 */
typedef void *(dup_func)(void const *const data);

/*!
 * @brief a function that frees an object from memory.
 *
 * @param[in] data pointer to the object to delete.
 */
typedef void(free_func)(void *const data);

/*!
 * @brief a function that stringifies an object.
 *
 * @param[in] data the object to stringify.
 *
 * @return pointer to the stringified data(must be null terminated),
 * NULL error.
 */
typedef char *(data_tostr)(void const *const data);

typedef struct list_node list_node;
typedef struct deque deque;

#endif /* BIGINT_LIST_TYPE_TYPEDEFS_H */
