#ifndef LINKED_NODES_H
#define LINKED_NODES_H

#include <stdlib.h> /* *alloc */
#include <stdio.h>	/* *printf */

#include "_typedefs.h"

/* singly linked list */

single_link_node *sln_new(void *const data, dup_func *duplicate_data);
single_link_node *sln_insert_after(
	single_link_node *const this_node, single_link_node *const other_node);
single_link_node *sln_insert_before(
	single_link_node *const this_node, single_link_node *other_node);
void *sln_remove(single_link_node *const node);
void *sln_get_data(single_link_node const *const node);
void *sln_swap_data(
	single_link_node *const node, void *const data, dup_func *copy_data);
single_link_node *sln_get_next(single_link_node const *const node);
void *sll_clear(single_link_node *const head, delete_func *free_data);
long int sll_print(
	FILE *stream, single_link_node const *const head, print_func *print_data);

/* doubly linked list */

double_link_node *dln_new(void *const data, dup_func *duplicate_data);
double_link_node *dln_insert_after(
	double_link_node *const this_node, double_link_node *const other_node);
double_link_node *dln_insert_before(
	double_link_node *const this_node, double_link_node *other_node);
void *dln_remove(double_link_node *node);
void *dln_get_data(double_link_node const *const node);
void *dln_swap_data(
	double_link_node *const node, void *const data, dup_func *copy_data);
double_link_node *dln_get_next(double_link_node const *const node);
double_link_node *dln_get_prev(double_link_node const *const node);
void *dll_clear(double_link_node *const head, delete_func *free_data);
long int dll_print(
	FILE *stream, double_link_node const *const head, print_func *print_data);
long int dll_print_reversed(
	FILE *stream, double_link_node const *const tail, print_func *print_data);

#endif /* LINKED_NODES_H */
