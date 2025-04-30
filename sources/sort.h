/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef SORT_H_
#define SORT_H_

#include "list.h"

typedef int (*cmp)(const BusEntity*, const BusEntity*);

/* Fonctions de comparaisons */
int compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b);
int compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b);

/* Unique fonction de tri */
List sort_list(List l, cmp cmp);

#endif // SORT_H_
