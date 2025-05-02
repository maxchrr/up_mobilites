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

static cmp curr_cmp = NULL;

static int wrapper(const void* a, const void* b)
{
	const BusEntity* ea = *(const BusEntity**)a;
	const BusEntity* eb = *(const BusEntity**)b;
	return curr_cmp(ea,eb);
}

BusLine sort_list(BusLine bl, cmp cmp)
{
	if (!bl || !cmp) return bl;
	curr_cmp = cmp;  // enregistrement temporaire du comparateur
	// Conversion en tableau
	int n = length(bl);
	BusEntity** arr = calloc(n,sizeof(BusEntity*));
	if (!arr) return bl;
	List head = bl;
	int i=0;
	while (!is_empty(head))
	{
		arr[i] = _get_node(head);
		head = _get_next_node(head);
		++i;
	}
	// Tri (quicksort)
	qsort(arr, n, sizeof(BusEntity*), wrapper);
	// Recréation de la liste
	BusLine sorted = NULL;
	init_list(&sorted);
	for (int i=0; i<n; ++i)
	{
		sorted = insert_at_tail(sorted, arr[i]);
	}
	free(arr);
	return sorted;
}
