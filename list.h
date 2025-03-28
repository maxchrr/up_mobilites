/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>

struct Node
{
	int* data; // Data held by the node - pointer to integer.
	struct Node* next;
	struct Node* prev;
};
typedef struct Node* List; // global variable - pointer to head node.

struct Node* _get_new_node(int x);
int _free_node(struct Node* n);

int init_list(List* l);
bool is_empty(List l);

List insert_at_head(List l, int x);
List insert_at_tail(List l, int x);
List insert(List l, int p, int x);

void print_list(List l);

List delete_at_head(List l);
List delete_at_tail(List l);
List delete(List l, int p);

struct Node* get_first_node(List l);
struct Node* get_last_node(List l);
struct Node* get_next_node(List l);
struct Node* get_prev_node(List l);
int* get_node(struct Node* n);

void swap_node(struct Node* n1, struct Node* n2);
int length(List l);
int sizeof_bytes(List l);

List merge(List l1, List l2);
List append(List l1, List l2);

struct Node* find_node(List l, int x);
int count_node(List l, int x);

#endif // LIST_H_
