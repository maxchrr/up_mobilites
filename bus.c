#include <stdio.h>
#include "bus.h"

void print_bus_line(Bus_Line_Object* bus_line)
{
	if (bus_line == NULL)
	{
		printf("This line didn't exist or not allocated.\n");
		return;
	}
	if (bus_line->bus_stop_or_route == BUS_STOP)
	{
		printf("Bus Stop %d \"%s\" at (%d,%d)\n",
			bus_line->id_bus_stop,
			bus_line->name,
			bus_line->pos_x,
			bus_line->pos_y);
	}
	else {
		printf("Bus Route %d (%dm) in %ds\n",
			bus_line->id_bus_route,
			bus_line->distance_due,
			bus_line->time_due);
		printf("From : "); print_bus_line(bus_line->departure);
		printf("To : "); print_bus_line(bus_line->arrival);
		printf("\n");
	}
	printf("End of Line\n");
}

int get_id_bus_stop(Bus_Line_Object* bus_line)
{
	return bus_line->id_bus_stop;
}

char* get_name(Bus_Line_Object* bus_line)
{
	return bus_line->name;
}

int get_pos_x(Bus_Line_Object* bus_line)
{
	return bus_line->pos_x;
}

int get_pos_y(Bus_Line_Object* bus_line)
{
	return bus_line->pos_y;
}

int get_id_bus_route(Bus_Line_Object* bus_line)
{
	if (bus_line->bus_stop_or_route == BUS_STOP)
		return -1;
	return bus_line->id_bus_route;
}

Bus_Line_Object_Type get_type(Bus_Line_Object* bus_line)
{
	return bus_line->bus_stop_or_route;
}
