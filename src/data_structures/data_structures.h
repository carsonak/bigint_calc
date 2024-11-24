#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * struct single_link_node - a singly linked list node.
 * @data: data that the node holds.
 * @next: pointer to the next node.
 */
typedef struct single_link_node
{
	void *data;
	struct single_link_node *next;
} single_link_nd;

/**
 * struct double_link_node - a doubly linked list node.
 * @data: data that the node holds.
 * @next: pointer to the next node.
 * @prev: pointer to the previous node.
 */
typedef struct double_link_node
{
	void *data;
	struct double_link_node *next;
	struct double_link_node *prev;
} double_link_node;

/*stack*/

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

single_link_nd *push(stack *s, void *data);
void *pop(stack *s);
void *clear_stack(stack *s, void *(*free_data)(void *));
/*void print_stack(stack *s, void (*print_data)(void *));*/

/*linked list*/

/**
 * struct link_list - a linked list data structure.
 * @size: the number of nodes in the linked list.
 * @head: pointer to the head of the linked list.
 */
typedef struct link_list
{
	size_t size;
	double_link_node *head;
} link_list;

double_link_node *insert_nd(double_link_node *node, void *data);
void *remove_nd(double_link_node *node);
void *clear_list(link_list *list, void *(*free_data)(void *));

/*deque*/

/**
 * struct deque - a deque data structure.
 * @size: number items in the deque.
 * @head: a pointer to the head of the deque.
 * @tail: a pointer to the tail of the deque.
 */
typedef struct deque
{
	size_t size;
	double_link_node *head;
	double_link_node *tail;
} deque;

double_link_node *push_head(deque *dq_head, void *data);
double_link_node *push_tail(deque *dq_head, void *data);
void *pop_head(deque *dq_head);
void *pop_tail(deque *dq_head);
void *clear_deque(deque *dq, void *(*free_data)(void *));
/*void print_deque(deque *dq_head, void (*print_data)(void *));*/

#endif /* DATA_STRUCTURES_H */
