/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include "api.h"

typedef struct Node
{
	BusEntity* data;    // Data held by the node - pointer on entity
	struct Node* next;
	struct Node* prev;
} Node;
typedef struct Node* List;

void  init_list(List* l);
void  print_list(List l);
void  destroy_list(List l);

List  _alloc_node(BusEntity* obj);
void  _free_node(Node* n);
bool  is_empty(List l);

List  insert_at_head(List l, BusEntity* obj);
List  insert_at_tail(List l, BusEntity* obj);
List  insert(List l, int p, BusEntity* obj);

List  delete_at_head(List l);
List  delete_at_tail(List l);
List  delete(List l, int p);

Node*       _get_first_node(List l);
Node*       _get_last_node(List l);
Node*       _get_next_node(List l);
Node*       _get_prev_node(List l);
BusEntity*  _get_node(Node* n);

void  swap_node(Node* n1, Node* n2);
int   length(List l);
int   sizeof_bytes(List l);

BusEntity*  _copy_entity(BusEntity* orig);
List        merge(List l1, List l2);
List        append(List l1, List l2);

Node*  find_node(List l, BusEntity* obj);
int    count_node(List l, BusEntity* obj);

#endif // LIST_H_
