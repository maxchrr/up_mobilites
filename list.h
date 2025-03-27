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

#endif // LIST_H_
