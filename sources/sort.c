/*
 * Module de tri et fonctions de comparaisons
 *
 * Copyright 2025 Max Charrier, Emilio Decaix-Massiani. Licensed under the Educational
 * Community License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/ECL-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include <stdlib.h>
#include "api.h"
#include "list.h"
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
	if (!source || !list_getnext_node(source))
	{
		*front_ref = source;
		*back_ref = NULL;
		return;
	}
	List slow = source;
	List fast = list_getnext_node(source);
	// slow avance de 1, fast avance de 2 => division
	while (fast)
	{
		fast = list_getnext_node(fast);
		if (fast)
		{
			slow = list_getnext_node(slow);
			fast = list_getnext_node(fast);
		}
	}
	// Division de la liste
	*front_ref = source;
	*back_ref = list_getnext_node(slow);
	slow->next = NULL;  // Point de séparation
}

static List sorted_merge(List a, List b, cmp comparator)
{
	if (!a) return b;
	if (!b) return a;
	List result = NULL;
	// Compare pour choisir qui de a ou b est le plus petit
	if (comparator(list_getnode(a), list_getnode(b)) <= 0)
	{
		result = a;  // a plus petit -> commence avec a
		result->next = sorted_merge(list_getnext_node(a), b, comparator);  // fusionne le reste de a avec b
	}
	else
	{
		result = b;  // b plus petit -> commence avec b
		result->next = sorted_merge(a, list_getnext_node(b), comparator);  // fusionne le reste de b avec a
	}
	return result;
}
List list_sort(List l, cmp comparator)
{
	if (!l || !list_getnext_node(l) || !comparator) return l;
	List a = NULL, b = NULL;
	split_list(l, &a, &b);
	// Trie des sous-listes récursif
	a = list_sort(a, comparator);
	b = list_sort(b, comparator);
	// Fusion des deux sous-listes non triée en une seule liste triée
	return sorted_merge(a, b, comparator);
}
