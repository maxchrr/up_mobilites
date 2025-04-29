/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdlib.h>
#include "api.h"
#include "sort.h"

int compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b)
{
	if (!a->station || !b->station) return 0;
	int cost_a = bs_getmaint_price(a->bs);
	int cost_b = bs_getmaint_price(b->bs);
	return cost_b - cost_a; // Tri décroissant
}
int compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b)
{
	if (!a->station || !b->station) return 0;
	Date d1 = bs_getlast_maint_date(a->bs);
	Date d2 = bs_getlast_maint_date(b->bs);
	if (d1.year != d2.year) return d1.year - d2.year;
	if (d1.month != d2.month) return d1.month - d2.month;
	return d1.day - d2.day;
}
