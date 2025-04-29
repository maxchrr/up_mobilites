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
	if (l == NULL) exit(EXIT_FAILURE);
	*l = NULL;
}

void print_list(List l)
{
	Node* temp = l;
	if (temp == NULL)
	{
		printf("List is empty\n");
		return;
	}
	while(temp != NULL)
	{
		print_entity(temp->data, 0);
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

bool is_empty(List l)
{
	return (l == NULL);
}

List insert_at_head(List l, BusEntity* obj)
{
	List new_node = _alloc_node(obj);
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

List insert_at_tail(List l, BusEntity* obj)
{
	List temp = l;
	List new_node = _alloc_node(obj);
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

List insert(List l, int p, BusEntity* obj)
{
	Node* temp = l;
	Node* new_node = _alloc_node(obj);
	if (is_empty(l))
	{
		l = new_node;
		return l;
	}
	if (p == 1)
		return insert_at_head(l, obj);
	for (int i=1; i<(p-1) && !is_empty(temp->next); ++i)
		temp = temp->next; // Go to the (p-1)-th Node (starting at 1)
	new_node->next = temp->next;
	new_node->prev = temp;
	if (!is_empty(temp->next))
		temp->next->prev = new_node;
	temp->next = new_node;
	return l;
}

List delete_at_head(List l)
{
	if (is_empty(l)) return l;
	Node* head = l->next; // Pointer to next Node
	if (!is_empty(head))
		head->prev = NULL;
	_free_node(l);
	l = head;
	return l;
}

List delete_at_tail(List l)
{
	if (is_empty(l)) return l;
	if (is_empty(l->next))
	{
		_free_node(l);
		return NULL;
	}
	Node* tail = l;
	while (!is_empty(tail->next))
		tail = tail->next; // Go to last Node
	if (!is_empty(tail->prev))
		tail->prev->next = NULL;
	_free_node(tail);
	return l;
}

List delete(List l, int p)
{
	Node* temp = l;
	if (p==1) return delete_at_head(l);
	for (int i=1; i<p && !is_empty(temp->next); ++i)
		temp = temp->next; // Go to the p-th Node (starting at 1)
	if (!is_empty(temp->prev))
		temp->prev->next = temp->next;
	if (!is_empty(temp->next))
		temp->next->prev = temp->prev;
	_free_node(temp);
	return l;
}

Node* _get_first_node(List l)
{
	return l;
}

Node* _get_last_node(List l)
{
	while (!is_empty(l->next)) l = l->next;
	return l;
}

Node* _get_next_node(List l)
{
	return l ? l->next : NULL;
}

Node* _get_prev_node(List l)
{
	return l ? l->prev : NULL;
}

BusEntity* _get_node(Node* n)
{
	if (is_empty(n))
	{
		fprintf(stderr, "Trying to access data of a NULL node\n");
		return NULL;
	}
	return n->data;
}

void swap_node(Node* n1, Node* n2)
{
	BusEntity* temp = _get_node(n1);
	n1->data = _get_node(n2);
	n2->data = temp;
}

int length(List l)
{
	int c=0;
	while (!is_empty(l))
	{
		++c;
		l = _get_next_node(l);
	}
	return c;
}

int sizeof_bytes(List l)
{
	return length(l)*sizeof(Node);
}

List merge(List l1, List l2)
{
	List new_list;
	init_list(&new_list); // Create a new empty List
	while (!is_empty(l1))
	{
		BusEntity* orig = _get_node(l1);
		BusEntity* copy = NULL;
		if (gettype(orig) == STATION)
		{
			BusStation* bs = orig->bs;
			copy = open_entity(1, create_bs(bs_getid(bs), bs_getname(bs), bs_getposx(bs), bs_getposy(bs)));
		}
		if (gettype(orig) == ROUTE)
		{
			BusRoute* br = orig->br;
			copy = open_entity(0, create_br(br_getbl_id(br), br_getdeparture(br), br_getarrival(br)));
		}
		new_list = insert_at_tail(new_list, copy);
		l1 = _get_next_node(l1); // Copy each Node of the first List
	}
	while (!is_empty(l2))
	{
		BusEntity* orig = _get_node(l2);
		BusEntity* copy = NULL;
		if (gettype(orig) == STATION)
		{
			BusStation* bs = orig->bs;
			copy = open_entity(1, create_bs(bs_getid(bs), bs_getname(bs), bs_getposx(bs), bs_getposy(bs)));
		}
		if (gettype(orig) == ROUTE)
		{
			BusRoute* br = orig->br;
			copy = open_entity(0, create_br(br_getbl_id(br), br_getdeparture(br), br_getarrival(br)));
		}
		new_list = insert_at_tail(new_list, copy);
		l2 = _get_next_node(l2); // Copy each Node of the second List
	}
	return new_list;
}

List append(List l1, List l2)
{
	if (is_empty(l1)) return l2;
	if (is_empty(l2)) return l1;
	Node* tail = _get_last_node(l1); // Go to last Node of the first List
	tail->next = l2;
	if (!is_empty(l2))
		l2->prev = tail; // Append the second List
	return l1;
}

Node* find_node(List l, BusEntity* obj)
{
	if (is_empty(l)) return NULL;
	while (_get_node(l) != obj && !is_empty(_get_next_node(l)))
		l = _get_next_node(l); // Get the first occurence of x
	return l;
}

int count_node(List l, BusEntity* obj)
{
	if (is_empty(l)) return 0;
	int c=0;
	while (!is_empty(l))
	{
		if (_get_node(l) == obj) ++c;
		l = _get_next_node(l);
	}
	return c;
}
