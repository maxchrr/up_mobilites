/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "list.h"

void init_list(List* l)
{
	if (l == NULL) return;
	*l = NULL;
}

void print_list(List l)
{
	Node* temp = l;
	if (temp == NULL)
	{
		fprintf(stderr, "List is empty\n");
		return;
	}
	while(temp != NULL)
	{
		print_entity(temp->data);
		temp = temp->next;
	}
}

void destroy_list(List l)
{
	Node* head = l;
	while (head != NULL)
	{
		List temp = head;
		head = temp->next;
		_free_node(temp);
	}
}

List _alloc_node(BusEntity* obj)
{
	List new_node = malloc(sizeof(Node));
	if (!new_node)
	{
		fprintf(stderr, "Memory allocation failed for the node\n");
		return NULL;
	}
	new_node->data = obj; // Set pointer as new pointer
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}

void _free_node(Node* n)
{
	if (n->data != NULL) close_entity(n->data);
	free(n);
}

bool list_is_empty(List l)
{
	return (l == NULL);
}

List insert_at_head(List l, BusEntity* obj)
{
	List new_node = _alloc_node(obj);
	if (list_is_empty(l))
	{
		l = new_node;
		return l;
	}
	l->prev = new_node;
	new_node->next = l;
	l = new_node;
	return l;
}

List insert_at_tail(List l, BusEntity* obj)
{
	List temp = l;
	List new_node = _alloc_node(obj);
	if (list_is_empty(l))
	{
		l = new_node;
		return l;
	}
	while (!list_is_empty(temp->next))
		temp = temp->next; // Go to last Node
	temp->next = new_node;
	new_node->prev = temp;
	return l;
}

List insert(List l, int p, BusEntity* obj)
{
	Node* temp = l;
	Node* new_node = _alloc_node(obj);
	if (list_is_empty(l))
	{
		l = new_node;
		return l;
	}
	if (p < 1) return l;  // Maybe outside the subset
	if (p == 1) return insert_at_head(l, obj);
	for (int i=1; i<(p-1) && !list_is_empty(temp->next); ++i)
		temp = temp->next; // Go to the (p-1)-th Node (starting at 1)
	new_node->next = temp->next;
	new_node->prev = temp;
	if (!list_is_empty(temp->next))
		temp->next->prev = new_node;
	temp->next = new_node;
	return l;
}

List delete_at_head(List l)
{
	if (list_is_empty(l)) return l;
	Node* head = l->next; // Pointer to next Node
	if (!list_is_empty(head))
		head->prev = NULL;
	_free_node(l);
	l = head;
	return l;
}

List delete_at_tail(List l)
{
	if (list_is_empty(l)) return l;
	if (list_is_empty(l->next))
	{
		_free_node(l);
		return NULL;
	}
	Node* tail = l;
	while (!list_is_empty(tail->next))
		tail = tail->next; // Go to last Node
	if (!list_is_empty(tail->prev))
		tail->prev->next = NULL;
	_free_node(tail);
	return l;
}

List delete(List l, int p)
{
	Node* temp = l;
	if (p==1) return delete_at_head(l);
	for (int i=1; i<p && !list_is_empty(temp->next); ++i)
		temp = temp->next; // Go to the p-th Node (starting at 1)
	if (!list_is_empty(temp->prev))
		temp->prev->next = temp->next;
	if (!list_is_empty(temp->next))
		temp->next->prev = temp->prev;
	_free_node(temp);
	return l;
}

Node* list_getfirst_node(List l)
{
	if (list_is_empty(l)) return NULL;
	return l;
}

Node* list_getlast_node(List l)
{
	if (list_is_empty(l)) return NULL;
	while (!list_is_empty(l->next)) l = l->next;
	return l;
}

Node* list_getnext_node(List l)
{
	return l ? l->next : NULL;
}

Node* list_getprev_node(List l)
{
	return l ? l->prev : NULL;
}

BusEntity* list_getnode(const Node* n)
{
	if (n == NULL)
	{
		fprintf(stderr, "Trying to access data of a NULL node\n");
		return NULL;
	}
	return n->data;
}

void swap_node(Node* n1, Node* n2)
{
	BusEntity* temp = list_getnode(n1);
	n1->data = list_getnode(n2);
	n2->data = temp;
}

int length(List l)
{
	int c=0;
	while (!list_is_empty(l))
	{
		++c;
		l = list_getnext_node(l);
	}
	return c;
}

int sizeof_bytes(const List l)
{
	return length(l)*sizeof(Node);
}

BusEntity* copy_entity(BusEntity* orig)
{
	if (gettype(orig) == STATION)
	{
		BusStation* bs = orig->bs;
		return open_entity(STATION, create_bs(bs_getid(bs), bs_getname(bs), bs_getposx(bs), bs_getposy(bs)));
	}
	if (gettype(orig) == ROUTE)
	{
		BusRoute* br = orig->br;
		return open_entity(ROUTE, create_br(br_getbl_id(br), br_getdeparture(br), br_getarrival(br)));
	}
	fprintf(stderr, "Unsupported entity type\n");
	return NULL;
}

List list_merge(List l1, List l2)
{
	List new_list;
	init_list(&new_list); // Create a new empty List
	while (!list_is_empty(l1))
	{
		BusEntity* orig = list_getnode(l1);
		BusEntity* copy = copy_entity(orig);
		new_list = insert_at_tail(new_list, copy);
		l1 = list_getnext_node(l1); // Copy each Node of the first List
	}
	while (!list_is_empty(l2))
	{
		BusEntity* orig = list_getnode(l2);
		BusEntity* copy = copy_entity(orig);
		new_list = insert_at_tail(new_list, copy);
		l2 = list_getnext_node(l2); // Copy each Node of the second List
	}
	return new_list;
}

List list_append(List l1, List l2)
{
	if (list_is_empty(l1)) return l2;
	if (list_is_empty(l2)) return l1;
	Node* tail = list_getlast_node(l1);  // Go to last Node of the first List
	Node* head = list_getfirst_node(l2);  // Go to first Node of the second List
	tail->next = head;  // Append the second List
	head->prev = tail;  // Preprend the first List
	return l1;
}

Node* find_node(List l, BusEntity* obj)
{
	if (list_is_empty(l)) return NULL;
	while (!list_is_empty(l))
	{
		if (list_getnode(l) == obj) return l;
		l = list_getnext_node(l); // Get the first occurence of x
	}
	return NULL;
}

int count_node(List l, BusEntity* obj)
{
	if (list_is_empty(l)) return 0;
	int c=0;
	while (!list_is_empty(l))
	{
		if (list_getnode(l) == obj) ++c;
		l = list_getnext_node(l);
	}
	return c;
}
