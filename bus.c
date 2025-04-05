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
			bus_line->bus_stop_id,
			bus_line->name,
			bus_line->pos_x,
			bus_line->pos_y);
	}
	else {
		printf("Bus Route %d (%dm) in %ds\n",
			bus_line->bus_route_id,
			bus_line->distance_due,
			bus_line->time_due);
		printf("From : "); print_bus_line((Bus_Line_Object*)bus_line->departure);
		printf("To : "); print_bus_line((Bus_Line_Object*)bus_line->arrival);
		printf("\n");
	}
	printf("End of Line\n");
}

/********************************
 *  Getter for Bus_Line_Object  *
 ********************************/
int get_bus_stop_id(Bus_Line_Object* bus_line)
{
	return bus_line->bus_stop_id;
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

int get_bus_route_id(Bus_Line_Object* bus_line)
{
	if (bus_line->bus_stop_or_route == BUS_STOP)
		return -1;
	return bus_line->bus_route_id;
}

Bus_Line_Object_Type get_type(Bus_Line_Object* bus_line)
{
	return bus_line->bus_stop_or_route;
}

int get_maintenance_price(Bus_Line_Object* bus_line)
{
	return bus_line->maintenance_price;
}

Maintenance_Date get_maintenance_date(Bus_Line_Object* bus_line)
{
	return bus_line->maintenance_date;
}

/********************************
 *  Setter for Bus_Line_Object  *
 ********************************/
void set_maintenance_price(Bus_Line_Object* bus_line, int new_price)
{
	bus_line->maintenance_price = new_price;
}

void set_maintenance_date(Bus_Line_Object* bus_line, Maintenance_Date new_date)
{
	bus_line->maintenance_date = new_date;
}

/********************************
 *     Getter for Bus_Object    *
 ********************************/
int get_bus_id(Bus_Object bus)
{
	return bus->bus_id;
}

int get_bus_pos_x(Bus_Object bus)
{
	return bus->pos_x;
}

int get_bus_pos_y(Bus_Object bus)
{
	return bus->pos_y;
}

int get_bus_line_id(Bus_Object bus)
{
	return bus->bus_line_id;
}

List_Bus_Line get_bus_line_pos(Bus_Object bus)
{
	return bus->bus_line_pos;
}

Bus_Line_Direction get_direction(Bus_Object bus)
{
	return bus->direction;
}

/********************************
 *     Setter for Bus_Object    *
 ********************************/
void set_bus_pos_x(Bus_Object bus, int new_pos_x)
{
	bus->pos_x = new_pos_x;
}

void set_bus_pos_y(Bus_Object bus, int new_pos_y)
{
	bus->pos_y = new_pos_y;
}

void set_bus_line_id(Bus_Object bus, int new_bus_line)
{
	bus->bus_line_id = new_bus_line;
}

void set_bus_line_pos(Bus_Object bus, List_Bus_Line bus_line)
{
	bus->bus_line_pos = bus_line;
}

void set_direction(Bus_Object bus, Bus_Line_Direction new_direction)
{
	bus->direction = new_direction;
}
