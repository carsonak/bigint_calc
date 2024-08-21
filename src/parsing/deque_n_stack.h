#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * struct single_link_node - a singly linked list node.
 * @next: pointer to the next node.
 * @data: data that the node holds.
 */
typedef struct single_link_node
{
	void *data;
	struct single_link_node *next;
} single_link_nd;

/**
 * struct double_link_node - a doubly linked list node.
 * @next: pointer to the next node.
 * @prev: pointer to the previous node.
 * @data: data that the node holds.
 */
typedef struct double_link_node
{
	void *data;
	struct double_link_node *next;
	struct double_link_node *prev;
} double_link_nd;

/**
 * struct stack - a stack data structure.
 * @size: the number of nodes in the stack.
 * @top: pointer to the top of the stack.
 */
typedef struct stack
{
	size_t size;
	single_link_nd *top;
} stack;

/**
 * struct deque - a deque data structure.
 * @size: number items in the deque.
 * @head: a pointer to the head of the deque.
 * @tail: a pointer to the tail of the deque.
 */
typedef struct deque
{
	size_t size;
	double_link_nd *head;
	double_link_nd *tail;
} deque;

/*stack*/

single_link_nd *push(stack *s, void *data);
void *pop(stack *s);
void *clear_stack(stack *s, void *(*free_data)(void *));
void print_stack(stack *s, void (*print_data)(void *));

/*deque*/

double_link_nd *push_head(deque *dq, void *data);
double_link_nd *push_tail(deque *dq, void *data);
void *pop_head(deque *dq);
void *pop_tail(deque *dq);
void *clear_deque(deque *dq, void *(*free_data)(void *));
void print_deque(deque *dq, void (*print_data)(void *));

#endif /* STACK_H */
