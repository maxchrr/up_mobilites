#include <stdio.h>
#include "raylib.h"
#include "api.h"
#include "list.h"

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	400

#define WINDOW_TITLE	"UPmobilites"

List create_bl(int id)
{
	BusStation* departure_station;
	BusStation* arrival_station;
	BusRoute* route;

	List new_bl;
	init_list(&new_bl);

	departure_station = create_bs(1, "Autoroute", 10, 20);
	arrival_station = create_bs(2, "Rivière", 20, 20);
	route = create_br(id, departure_station, arrival_station, 500, 1200);

	new_bl = insert_at_tail(new_bl, open_entity(1, departure_station));
	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, arrival_station));

	departure_station = create_bs(3, "Centre ville", 30, 20);
	route = create_br(id, arrival_station, departure_station, 800, 1600);

	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, departure_station));

	arrival_station = create_bs(4, "Université", 40, 20);
	route = create_br(id, departure_station, arrival_station, 200, 800);

	new_bl = insert_at_tail(new_bl, open_entity(0, route));
	new_bl = insert_at_tail(new_bl, open_entity(1, arrival_station));

	departure_station = create_bs(5, "Ville Sud", 50, 20);
	route = create_br(id, arrival_station, departure_station, 2400, 4000);

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
