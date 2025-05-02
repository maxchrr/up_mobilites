/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef SORT_H_
#define SORT_H_

#include <stdbool.h>
#include "list.h"

typedef int (*cmp)(const BusEntity*, const BusEntity*);

/* Fonctions de comparaisons */
bool compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b);
bool compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b);

/* Unique fonction de tri */
List sort_list(List l, bool (*cmp)(const BusEntity*, const BusEntity*));

#endif // SORT_H_
