/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include "bus.h"

struct Node
{
	Bus_Line_Object* data; // Data held by the node - pointer to integer.
	struct Node* next;
	struct Node* prev;
};
typedef struct Node* List_Bus_Line; // global variable - pointer to head node.


struct Node* _get_new_node(Bus_Line_Object* x);
int _free_node(struct Node* n);

int init_list(List_Bus_Line* l);
bool is_empty(List_Bus_Line l);

List_Bus_Line insert_at_head(List_Bus_Line l, Bus_Line_Object* x);
List_Bus_Line insert_at_tail(List_Bus_Line l, Bus_Line_Object* x);
List_Bus_Line insert(List_Bus_Line l, int p, Bus_Line_Object* x);

void print_list(List_Bus_Line l);

List_Bus_Line delete_at_head(List_Bus_Line l);
List_Bus_Line delete_at_tail(List_Bus_Line l);
List_Bus_Line delete(List_Bus_Line l, int p);

struct Node* get_first_node(List_Bus_Line l);
struct Node* get_last_node(List_Bus_Line l);
struct Node* get_next_node(List_Bus_Line l);
struct Node* get_prev_node(List_Bus_Line l);
Bus_Line_Object* get_node(struct Node* n);

void swap_node(struct Node* n1, struct Node* n2);
int length(List_Bus_Line l);
int sizeof_bytes(List_Bus_Line l);

#endif // LIST_H_
