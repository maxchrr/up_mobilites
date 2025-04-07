#include <stdio.h>
#include "list.h"
#include "sort.h"
#include "utils.h"

int main(void)
{
	List_Bus_Line line;
	Bus_Line_Object* departure;
	Bus_Line_Object* arrival;
	Bus_Line_Object* route;
	init_list(&line);

	departure = create_stop(1,"Foo",0,0);
	arrival = create_stop(2,"Bar",10,2);
	route = create_route(1,departure,arrival,280,35);

	line = insert_at_tail(line, departure);
	line = insert_at_tail(line, route);
	line = insert_at_tail(line, arrival);

	departure = create_stop(3,"Baz",20,4);
	route = create_route(1,arrival,departure,160,40);
	line = insert_at_tail(line, route);
	line = insert_at_tail(line, departure);

	arrival = create_stop(4,"Qux",30,6);
	route = create_route(1,departure,arrival,200,45);
	line = insert_at_tail(line, route);
	line = insert_at_tail(line, arrival);

	departure = create_stop(5,"Quux",40,8);
	route = create_route(1,arrival,departure,160,40);
	line = insert_at_tail(line, route);
	line = insert_at_tail(line, departure);
	print_list(line);
	return 0;
}
