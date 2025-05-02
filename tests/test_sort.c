/*
 * Test unitaire des fonctions de tri
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "bus.h"
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
	BusLine bl;
	init_list(&bl);
	Date d1 = {5, 5, 2021};
	Date d2 = {15, 3, 2023};
	Date d3 = {20, 7, 2019};
	bl = insert_at_tail(bl, open_entity(STATION, mock_station(1, "Alpha", 50, d1)));
	bl = insert_at_tail(bl, open_entity(STATION, mock_station(2, "Bravo", 80, d2)));
	bl = insert_at_tail(bl, open_entity(STATION, mock_station(3, "Charlie", 30, d3)));

	printf("Avant tri par coût de maintenance décroissant :\n");
	print_list(bl);

	bl = sort_list(bl, compare_by_maint_price_desc);

	printf("\nAprès tri par coût de maintenance décroissant :\n");
	print_list(bl);

	bl = sort_list(bl, compare_by_last_maint_date_asc);

	printf("\nAprès tri par date de maintenance croissante :\n");
	print_list(bl);

	destroy_list(bl);
	return 0;
}
