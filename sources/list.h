/*
 * Doubly Linked List implementation
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include "api.h"

/* API funcitons that operate on Node objects */
struct Node* _get_new_node(struct Bus_Line* x);
int _free_node(struct Node* n);

int init_list(List* l);
bool is_empty(List l);

List insert_at_head(List l, struct Bus_Line* x);
List insert_at_tail(List l, struct Bus_Line* x);
List insert(List l, int p, struct Bus_Line* x);

void _print_list(List l);

List delete_at_head(List l);
List delete_at_tail(List l);
List delete(List l, int p);

struct Node* _get_first_node(List l);
struct Node* _get_last_node(List l);
struct Node* _get_next_node(List l);
struct Node* _get_prev_node(List l);
struct Bus_Line* _get_node(struct Node* n);

void swap_node(struct Node* n1, struct Node* n2);
int length(List l);
int sizeof_bytes(List l);

List merge(List l1, List l2);
List append(List l1, List l2);

struct Node* find_node(List l, struct Bus_Line* x);
int count_node(List l, struct Bus_Line* x);

#endif // LIST_H_
