/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sort.h"

bool compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b)
{
	if (gettype(a) != STATION || gettype(b) != STATION) return 0;
	int cost_a = bs_getmaint_price(a->bs);
	int cost_b = bs_getmaint_price(b->bs);
	return cost_b < cost_a;
}

bool compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b)
{
	if (gettype(a) != STATION || gettype(b) != STATION) return 0;
	Date d1 = bs_getlast_maint_date(a->bs);
	Date d2 = bs_getlast_maint_date(b->bs);
	if (d1.year != d2.year) return d1.year - d2.year;
	if (d1.month != d2.month) return d1.month - d2.month;
	return d1.day > d2.day;
}

List _merge(List l, bool (*cmp)(const BusEntity*, const BusEntity*))
{
	if (!is_empty(l) || !is_empty(_get_next_node(l))) return l;
	List slow = l, fast = _get_next_node(l);
	while (fast && _get_next_node(fast))
	{
		slow = _get_next_node(slow);
		fast = _get_next_node(_get_next_node(fast));
	}
	List temp = _get_next_node(slow);
	slow->next = NULL;
	if (temp) temp->prev = NULL;
	List left = _merge(l, cmp);
	List right = _merge(temp, cmp);
	List head = NULL, tail = NULL;
	while (left && right)
	{
		bool swap = cmp(_get_node(left), _get_node(right));
		List selected = swap ? right : left;
		if (!head)
		{
			head = selected;
			tail = selected;
		}
		else
		{
			tail->next = selected;
			selected->prev = tail;
			tail = selected;
		}
		if (swap)	right = _get_next_node(right);
		else		left = _get_next_node(left);
	}
	List rest = left ? left : right;
	while (rest)
	{
		tail->next = rest;
		rest->prev = tail;
		tail = rest;
		rest = _get_next_node(rest);
	}
	if (head) head->prev = NULL;
	if (tail) tail->prev = NULL;
	return head;
}

List sort_list(List l, bool (*cmp)(const BusEntity*, const BusEntity*))
{
	if (is_empty(l) || is_empty(_get_next_node(l))) return l;
	return _merge(l, cmp);
}
