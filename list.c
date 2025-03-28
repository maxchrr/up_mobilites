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
	if (!is_empty(temp->next))
		temp->next->prev = new_node;
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

List delete_at_head(List l) {
	if (is_empty(l))
		return l;
	struct Node* head = l->next; // Pointer to next Node
	head->prev = NULL;
	_free_node(l);
	l = head;
	return l;
}

List delete_at_tail(List l) {
	struct Node* temp = l;
	while (!is_empty(temp->next))
		temp = temp->next; // Go to last Node
	temp->prev->next = NULL;
	_free_node(temp);
	return l;
}

List delete(List l, int p) {
	struct Node* temp = l;
	if (p==1)
		return delete_at_head(l);
	for (int i=1; i<p && !is_empty(temp->next); ++i)
		temp = temp->next; // Go to the p-th node (starting at 1)
	if (!is_empty(temp->prev))
		temp->prev->next = temp->next;
	if (!is_empty(temp->next))
		temp->next->prev = temp->prev;
	_free_node(temp);
	return l;
}

struct Node* get_first_node(List l)
{
	return l;
}

struct Node* get_last_node(List l)
{
	while (!is_empty(l->next))
		l = l->next;
	return l;
}

struct Node* get_next_node(List l)
{
	if (is_empty(l->next))
		return NULL;
	return l->next;
}

struct Node* get_prev_node(List l)
{
	if (is_empty(l->prev))
		return NULL;
	return l->prev;
}

int* get_data(List l)
{
	return l->data;
}
