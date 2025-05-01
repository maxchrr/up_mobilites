/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdlib.h>
#include "api.h"
#include "sort.h"

bool compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b)
{
	if (gettype(a) != STATION || gettype(b) != STATION) return 0;
	int cost_a = bs_getmaint_price(a->bs);
	int cost_b = bs_getmaint_price(b->bs);
	return cost_a < cost_b;
}

bool compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b)
{
	if (gettype(a) != STATION || gettype(b) != STATION) return 0;
	Date d1 = bs_getlast_maint_date(a->bs);
	Date d2 = bs_getlast_maint_date(b->bs);
	if (d1.year != d2.year) return d1.year - d2.year;
	if (d1.month != d2.month) return d1.month - d2.month;
	return d1.year > d2.year || 
       (d1.year == d2.year && d1.month > d2.month) || 
       (d1.year == d2.year && d1.month == d2.month && d1.day > d2.day);
}

List sort_list(List l, bool (*compare)(const BusEntity*, const BusEntity*)) {
	if (l == NULL || l->next == NULL) return l;

	List merge_sort_internal(List list) {
		if (!list || !list->next) return list;

		List slow = list, fast = list->next;
		while (fast && fast->next) {
			slow = slow->next;
			fast = fast->next->next;
		}

		List second = slow->next;
		slow->next = NULL;
		if (second) second->prev = NULL;

		List left = merge_sort_internal(list);
		List right = merge_sort_internal(second);

		List head = NULL, tail = NULL;

		while (left && right) {
			bool swap = compare(left->data, right->data);
			Node* selected = swap ? right : left;

			if (!head) {
				head = selected;
				tail = selected;
			} else {
				tail->next = selected;
				selected->prev = tail;
				tail = selected;
			}

			if (swap) right = right->next;
			else left = left->next;
		}

		List rest = left ? left : right;
		while (rest) {
			tail->next = rest;
			rest->prev = tail;
			tail = rest;
			rest = rest->next;
		}

		if (head) head->prev = NULL;
		if (tail) tail->next = NULL;

		return head;
	}

	return merge_sort_internal(l);
}
