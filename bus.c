#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bus.h"
#include "list.h"

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
		printf("-- From : "); print_bus_line((Bus_Line_Object*)bus_line->departure);
		printf("-- To : "); print_bus_line((Bus_Line_Object*)bus_line->arrival);
	}
}

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

void set_maintenance_price(Bus_Line_Object* bus_line, int new_price)
{
	bus_line->maintenance_price = new_price;
}

void set_maintenance_date(Bus_Line_Object* bus_line, Maintenance_Date new_date)
{
	bus_line->maintenance_date = new_date;
}

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

Bus_Object create_bus(int id)//, List_Bus_Line start)
{
	Bus_Object new_bus = malloc(sizeof(Bus_Object));
	new_bus->bus_id = id;
	return new_bus;
}

Bus_Line_Object* create_stop(int id, char* name, int pos_x, int pos_y)
{
	Bus_Line_Object* new_stop = malloc(sizeof(Bus_Line_Object));
	if (!new_stop)
	{
		printf("Memory allocation failed for BUS_STOP.\n");
		return NULL;
	}
	new_stop->bus_stop_or_route = BUS_STOP;
	// Fields for BUS_STOP
	new_stop->bus_stop_id = id;
	strcpy(new_stop->name, name); // Set pointer as new pointer
	new_stop->pos_x = pos_x;
	new_stop->pos_y = pos_y;
	// Useless fields for BUS_ROUTE
	new_stop->bus_route_id = -1; // NULL ID
	new_stop->departure = NULL;
	new_stop->arrival = NULL;
	new_stop->distance_due = 0;
	new_stop->time_due = 0;
	return new_stop;
}

Bus_Line_Object* create_route(
	int id,
	Bus_Line_Object* departure,
	Bus_Line_Object* arrival,
	int distance_due,
	int time_due
)
{
	Bus_Line_Object* new_route = malloc(sizeof(Bus_Line_Object));
	if (!new_route)
	{
		printf("Memory allocation failed for BUS_STOP.\n");
		return NULL;
	}
	new_route->bus_stop_or_route = BUS_ROUTE;
	// Fields for BUS_ROUTE
	new_route->bus_route_id = id; // NULL ID
	new_route->departure = (struct Bus_Line_Object*)departure;
	new_route->arrival = (struct Bus_Line_Object*)arrival;
	new_route->distance_due = distance_due;
	new_route->time_due = time_due;
	// Useless ields for BUS_STOP
	new_route->bus_stop_id = -1; // NULL ID
	strcpy(new_route->name, "");
	new_route->pos_x = 0;
	new_route->pos_y = 0;
	return new_route;
}

struct Node* get_next_stop(List_Bus_Line l)
{
	if (is_empty(l))
		return NULL;
	if (get_type(_get_node(l)) == BUS_STOP)
	{
		if (is_empty(_get_next_node(l)))
			return NULL; // Terminal station
		else
			return _get_next_node(_get_next_node(l));
			// Iter over two nodes, next one is BUS_ROUTE, then BUS_STOP
	}
	else
		return _get_next_node(l); // Next node is BUS_STOP
}

struct Node* get_prev_stop(List_Bus_Line l)
{
	if (is_empty(l))
		return NULL;
	if (get_type(_get_node(l)) == BUS_ROUTE)
	{
		if (is_empty(_get_prev_node(l)))
			return NULL; // Terminal station
		else
			return _get_prev_node(_get_prev_node(l));
			// Iter backwards over two nodes, previous one is BUS_ROUTE, then BUS_STOP
	}
	else
		return _get_prev_node(l); // Previous node is BUS_STOP
}

struct Node* get_next_route(List_Bus_Line l)
{
	if (is_empty(l))
		return NULL;
	if (get_type(_get_node(l)) == BUS_ROUTE)
		return _get_next_node(l); // Next node is BUS_ROUTE
	return l;
}

struct Node* get_prev_route(List_Bus_Line l)
{
	if (is_empty(l))
		return NULL;
	if (get_type(_get_node(l)) == BUS_ROUTE)
		return _get_prev_node(l); // Previous node is BUS_ROUTE
	return l;
}

int get_pos_x_in_list(List_Bus_Line l)
{
	return get_pos_x(_get_node(l));
}

int get_pos_y_in_list(List_Bus_Line l)
{
	return get_pos_y(_get_node(l));
}
