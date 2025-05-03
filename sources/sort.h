/*
 * Fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#include "list.h"

typedef int (*cmp)(const BusEntity*, const BusEntity*);

/* Fonctions de comparaisons */
int  compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b);
int  compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b);

/* Unique fonction de tri */
List  list_sort(List l, cmp comparator);
