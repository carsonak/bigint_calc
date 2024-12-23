#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdio.h>

/**
 * dup_func - a function that duplicates an object.
 * @data: pointer to the object to be duplicated.
 *
 * Return: pointer to the duplicate, NULL on failure.
 */
typedef void *(dup_func)(void const *const data);

/**
 * delete_func - a function that deletes an object.
 * @data: pointer to the object to delete.
 */
typedef void(delete_func)(void *const data);

/**
 * print_func - a function that prints an object.
 * @stream: pointer to a stream to print to.
 * @data: the object to print.
 *
 * Return: number of characters printed, negative number on error.
 */
typedef int(print_func)(FILE *stream, void const *const data);

typedef struct single_link_node single_link_node;
typedef struct double_link_node double_link_node;

typedef struct queue queue;
typedef struct deque deque;
typedef struct stack stack;

#endif /*TYPEDEFS_H*/
