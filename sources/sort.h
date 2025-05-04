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
#pragma once

#include "list.h"

typedef int (*cmp)(const BusEntity*, const BusEntity*);

/* Fonctions de comparaisons */
int  compare_by_maint_price_desc(const BusEntity* a, const BusEntity* b);
int  compare_by_last_maint_date_asc(const BusEntity* a, const BusEntity* b);

/* Unique fonction de tri */
List  list_sort(List l, cmp comparator);
