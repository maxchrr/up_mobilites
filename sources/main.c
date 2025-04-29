/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include "api.h"
#include "list.h"

List create_bl(int id)
{
	BusStation* departure_station;
	BusStation* arrival_station;
	BusRoute* route;

	List new_bl;
	init_list(&new_bl);

	departure_station = create_bs(1, "Autoroute", 10, 20);
	arrival_station = create_bs(2, "Rivière", 40, 20);
	route = create_br(id, departure_station, arrival_station);

	new_bl = insert_at_tail(new_bl, open_entity(1, departure_station));
	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, arrival_station));

	departure_station = create_bs(3, "Centre ville", 150, 20);
	route = create_br(id, arrival_station, departure_station);

	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, departure_station));

	arrival_station = create_bs(4, "Université", 599, 50);
	route = create_br(id, departure_station, arrival_station);

	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, arrival_station));

	departure_station = create_bs(5, "Ville Sud", 843, 80);
	route = create_br(id, arrival_station, departure_station);

	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, departure_station));

	return new_bl;
}

int main(void)
{
	List bl = create_bl(1001);
	print_list(bl);
	destroy_list(bl);
	return 0;
}
