#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bus.h"
#include "list.h"
#include "utils.h"

void print_bus_object(Bus_Line_Object* bus_line, int indent)
{
	if (bus_line == NULL)
	{
		printf("%*sThis line didn't exist or not allocated.\n", indent, "");
		return;
	}
	if (bus_line->bus_stop_or_route == BUS_STOP)
	{
		printf("%*sBus Stop %d \"%s\" at (%d,%d)\n",
			indent, "",
			bus_line->bus_stop_id,
			bus_line->name,
			bus_line->pos_x,
			bus_line->pos_y);
		printf("%*sLast maintenance: %d€ on %02d/%02d/%04d\n",
			indent+2, "",
			bus_line->maintenance_price,
			bus_line->maintenance_date.day,
			bus_line->maintenance_date.month,
			bus_line->maintenance_date.year);
	}
	else {
		printf("%*sBus Route %d (%dm) in %ds\n",
			indent, "",
			bus_line->bus_route_id,
			bus_line->distance_due,
			bus_line->time_due);
		printf("%*s-- From : ", indent, ""); print_bus_object((Bus_Line_Object*)bus_line->departure, indent+4);
		printf("%*s-- To : ", indent, ""); print_bus_object((Bus_Line_Object*)bus_line->arrival, indent+4);
	}
}

int get_bus_stop_id(Bus_Line_Object* bus_line)
{
	return bus_line->bus_stop_id;
}

char* get_bus_stop_name(Bus_Line_Object* bus_line)
{
	return bus_line->name;
}

int get_bus_stop_pos_x(Bus_Line_Object* bus_line)
{
	return bus_line->pos_x;
}

int get_bus_stop_pos_y(Bus_Line_Object* bus_line)
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

Bus_Object create_bus(int id, List_Bus_Line start)
{
	Bus_Object new_bus = malloc(sizeof(Bus_Object));
	new_bus->bus_id = id;
	set_bus_on_bus_line(new_bus, start, DEP_TO_ARR);
	int new_bus_line = get_bus_route_id(_get_node(get_next_route(start)));
	new_bus->bus_line_id = new_bus_line;
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
	memset(new_stop, 0, sizeof(Bus_Line_Object));
	new_stop->bus_stop_or_route = BUS_STOP;
	// Fields for BUS_STOP
	new_stop->bus_stop_id = id;
	strncpy(new_stop->name, name, sizeof(new_stop->name)); // Set pointer as new pointer
	new_stop->name[sizeof(new_stop->name) - 1] = '\0'; // Ensure null-termination
	new_stop->pos_x = pos_x;
	new_stop->pos_y = pos_y;
	new_stop->maintenance_price = rand_range(10,100);
	int new_year = rand_range(2018,2024);
	int new_month = rand_range(1,12);
	int new_day = rand_range(1, (new_month == 2) ? (((new_year % 400 == 0) || (new_year % 4 == 0 && !(new_year % 100 == 0))) ? 29 : 28) : ((new_month == 4 || new_month == 6 || new_month == 9 || new_month == 11) ? 30 : 31));
	Maintenance_Date new_maintenance_date;
	new_maintenance_date.day = new_day;
	new_maintenance_date.month = new_month;
	new_maintenance_date.year = new_year;
	new_stop->maintenance_date = new_maintenance_date;
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
	memset(new_route, 0, sizeof(Bus_Line_Object));
	new_route->bus_stop_or_route = BUS_ROUTE;
	// Fields for BUS_ROUTE
	new_route->bus_route_id = id; // NULL ID
	new_route->departure = (struct Bus_Line_Object*)departure;
	new_route->arrival = (struct Bus_Line_Object*)arrival;
	new_route->distance_due = distance_due;
	new_route->time_due = time_due;
	// Useless fields for BUS_STOP
	new_route->bus_stop_id = -1; // NULL ID
	strncpy(new_route->name, "", sizeof(new_route->name));
	new_route->pos_x = 0;
	new_route->pos_y = 0;
	new_route->maintenance_price = 0;
	Maintenance_Date new_maintenance_date;
	new_maintenance_date.day = 0;
	new_maintenance_date.month = 0;
	new_maintenance_date.year = 0;
	new_route->maintenance_date = new_maintenance_date;
	return new_route;
}

void print_bus_line(List_Bus_Line l)
{
	_print_list(l);
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
	return get_bus_stop_pos_x(_get_node(l));
}

int get_pos_y_in_list(List_Bus_Line l)
{
	return get_bus_stop_pos_y(_get_node(l));
}

void set_bus_on_bus_line(Bus_Object bus, List_Bus_Line bus_line, Bus_Line_Direction direction)
{
	set_bus_line_pos(bus, bus_line);
	set_direction(bus, direction);
	set_bus_pos_x(bus, get_pos_x_in_list(bus_line));
	set_bus_pos_y(bus, get_pos_y_in_list(bus_line));
	printf("Bus %d at (%d,%d) on Bus Route %d / Bus Stop %s\n",
		get_bus_id(bus),
		get_bus_pos_x(bus),
		get_bus_pos_y(bus),
		get_bus_line_id(bus),
		get_bus_stop_name(_get_node(bus_line))
	);
}
