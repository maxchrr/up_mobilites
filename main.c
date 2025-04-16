#include <stdio.h>
#include "bus.h"
#include "sort.h"
#include "utils.h"

List_Bus_Line create_bus_line(void)
{
	List_Bus_Line new_line = NULL;
	Bus_Line_Object* departure;
	Bus_Line_Object* arrival;
	Bus_Line_Object* route;
	init_list(&new_line);

	departure = create_stop(1,"Foo",0,0);
	arrival = create_stop(2,"Bar",10,2);
	route = create_route(1,departure,arrival,280,35);

	new_line = insert_at_tail(new_line, departure);
	new_line = insert_at_tail(new_line, route);
	new_line = insert_at_tail(new_line, arrival);

	departure = create_stop(3,"Baz",20,4);
	route = create_route(1,arrival,departure,160,40);
	new_line = insert_at_tail(new_line, route);
	new_line = insert_at_tail(new_line, departure);

	arrival = create_stop(4,"Qux",30,6);
	route = create_route(1,departure,arrival,200,45);
	new_line = insert_at_tail(new_line, route);
	new_line = insert_at_tail(new_line, arrival);

	departure = create_stop(5,"Quux",40,8);
	route = create_route(1,arrival,departure,160,40);
	new_line = insert_at_tail(new_line, route);
	new_line = insert_at_tail(new_line, departure);

	return new_line;
}

int main(void)
{
	List_Bus_Line bus_line1 = create_bus_line();
	print_bus_line(bus_line1);

	return 0;
}
