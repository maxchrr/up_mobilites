/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdlib.h>
#include "api.h"
#include "bus.h"
#include "sort.h"

int compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b)
{
	if (!a || !b || gettype(a) != STATION || gettype(b) != STATION) return 0;
	int pa = bs_getmaint_price(a->bs);
	int pb = bs_getmaint_price(b->bs);
	return pb - pa;  // décroissant
}

int compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b)
{
	if (!a || !b || gettype(a) != STATION || gettype(b) != STATION) return 0;
	Date da = bs_getlast_maint_date(a->bs);
	Date db = bs_getlast_maint_date(b->bs);
	if (da.year != db.year)		return da.year - db.year;
	if (da.month != db.month)	return da.month - db.month;
	return				da.day - db.day;
}

static void split_list(List source, List* front_ref, List* back_ref)
{
	if (!source || !_get_next_node(source))
	{
		*front_ref = source;
		*back_ref = NULL;
		return;
	}
	List slow = source;
	List fast = _get_next_node(source);
	// slow avance de 1, fast avance de 2 => division
	while (fast)
	{
		fast = _get_next_node(fast);
		if (fast)
		{
			slow = _get_next_node(slow);
			fast = _get_next_node(fast);
		}
	}
	// Division de la liste
	*front_ref = source;
	*back_ref = _get_next_node(slow);
	slow->next = NULL;  // Point de séparation
}

static List sorted_merge(List a, List b, cmp comparator)
{
	if (!a) return b;
	if (!b) return a;
	List result = NULL;
	// Compare pour choisir qui de a ou b est le plus petit
	if (comparator(_get_node(a), _get_node(b)) <= 0)
	{
		result = a;  // a plus petit -> commence avec a
		result->next = sorted_merge(_get_next_node(a), b, comparator);  // fusionne le reste de a avec b
	}
	else
	{
		result = b;  // b plus petit -> commence avec b
		result->next = sorted_merge(a, _get_next_node(b), comparator);  // fusionne le reste de b avec a
	}
	return result;
}
BusLine sort_list(BusLine bl, cmp comparator)
{
	if (!bl || !_get_next_node(bl) || !comparator) return bl;
	List a = NULL, b = NULL;
	split_list(bl, &a, &b);
	// Trie des sous-listes récursif
	a = sort_list(a, comparator);
	b = sort_list(b, comparator);
	// Fusion des deux sous-listes non triée en une seule liste triée
	return sorted_merge(a, b, comparator);
}
