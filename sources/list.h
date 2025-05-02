/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#pragma once

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
bool  list_is_empty(List l);

List  insert_at_head(List l, BusEntity* obj);
List  insert_at_tail(List l, BusEntity* obj);
List  insert(List l, int p, BusEntity* obj);

List  delete_at_head(List l);
List  delete_at_tail(List l);
List  delete(List l, int p);

Node*       list_getfirst_node(List l);
Node*       list_getlast_node(List l);
Node*       list_getnext_node(List l);
Node*       list_getprev_node(List l);
BusEntity*  list_getnode(const Node* n);

void  swap_node(Node* n1, Node* n2);
int   length(List l);
int   sizeof_bytes(const List l);

BusEntity*  copy_entity(BusEntity* orig);
List        list_merge(List l1, List l2);
List        list_append(List l1, List l2);

Node*  find_node(List l, BusEntity* obj);
int    count_node(List l, BusEntity* obj);
