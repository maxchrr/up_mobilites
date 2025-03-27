/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

struct Node* _get_new_node(int x)
{
	struct Node* new_node = malloc(sizeof(struct Node));
	if (!new_node)
	{
		printf("Memory allocation failed for the node.\n");
		return NULL;
	}
	new_node->data = malloc(sizeof(int));
	if (!new_node->data)
	{
		printf("Memory allocation failed for the node data.\n");
		free(new_node);
		return NULL;
	}
	*(new_node->data) = x;
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}

int _free_node(struct Node* n) {
	free(n->data);
	free(n);
	return 0;
}

int init_list(List* l)
{
	if (l == NULL)
		exit(EXIT_FAILURE);
	*l = NULL;
	return 0;
}

bool is_empty(List l)
{
	return (l == NULL);
}

List insert_at_head(List l, int x) {
	struct Node* new_node = _get_new_node(x);
	if (is_empty(l))
	{
		l = new_node;
		return l;
	}
	l->prev = new_node;
	new_node->next = l;
	l = new_node;
	return l;
}

List insert_at_tail(List l, int x) {
	struct Node* temp = l;
	struct Node* new_node = _get_new_node(x);
	if (is_empty(l))
	{
		l = new_node;
		return l;
	}
	while (!is_empty(temp->next))
		temp = temp->next; // Go to last Node
	temp->next = new_node;
	new_node->prev = temp;
	return l;
}

List insert(List l, int p, int x) {
	struct Node* temp = l;
	struct Node* new_node = _get_new_node(x);
	if (is_empty(l))
	{
		l = new_node;
		return l;
	}
	if (p == 1)
		return insert_at_head(l, x);
	for (int i=1; i<(p-1) && !is_empty(temp->next); ++i)
		temp = temp->next; // Go to the (p-1)-th node (starting at 1)
	new_node->next = temp->next;
	new_node->prev = temp;
	if (temp->next)
	{
		struct Node* ptr = temp->next;
		temp->next = new_node;
	}
	temp->next = new_node;
	return l;
}

void print_list(List l) {
	struct Node* temp = l;
	if (is_empty(temp))
	{
		printf("List is empty.\n");
		return;
	}
	while(temp != NULL)
	{
		printf("%d ",*(temp->data));
		temp = temp->next;
	}
	printf("\n");
}
