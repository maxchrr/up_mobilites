/*
 * Test unitaire des fonctions de comparaison et tri
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
#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "list.h"
#include "sort.h"

BusStation* mock_station(int id, const char* name, int price, Date date)
{
	BusStation* bs = create_bs(id, name, 0, 0);
	// Contrôle des données
	bs_setmaint_price(bs, price);
	bs_setlast_maint_date(bs, date);
	return bs;
}

int main(void)
{
	List list;
	init_list(&list);
	Date d1 = {5, 5, 2021};
	Date d2 = {15, 3, 2023};
	Date d3 = {20, 7, 2019};
	list = insert_at_tail(list, open_entity(STATION, mock_station(1, "Alpha", 50, d1)));
	list = insert_at_tail(list, open_entity(STATION, mock_station(2, "Bravo", 80, d2)));
	list = insert_at_tail(list, open_entity(STATION, mock_station(3, "Charlie", 30, d3)));

	printf("Avant tri par coût de maintenance décroissant :\n");
	print_list(list);

	list = list_sort(list, compare_by_maint_price_desc);

	printf("\nAprès tri par coût de maintenance décroissant :\n");
	print_list(list);

	list = list_sort(list, compare_by_last_maint_date_asc);

	printf("\nAprès tri par date de maintenance croissante :\n");
	print_list(list);

	destroy_list(list);
	return 0;
}
