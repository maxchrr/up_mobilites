#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "sort.h"

bool _is_date_more_recent(Bus_Line_Object* bl1, Bus_Line_Object* bl2)
{
	Maintenance_Date d_bl1 = get_maintenance_date(bl1);
	Maintenance_Date d_bl2 = get_maintenance_date(bl2);
	if (d_bl1.year != d_bl2.year) return d_bl1.year < d_bl2.year;
	if (d_bl1.month != d_bl2.month) return d_bl1.month < d_bl2.month;
	return d_bl1.day < d_bl2.day;
}


bool _is_price_higher(Bus_Line_Object* bl1, Bus_Line_Object* bl2)
{
	int p_bl1 = get_maintenance_price(bl1);
	int p_bl2 = get_maintenance_price(bl2);
	return(p_bl1 > p_bl2);
}

void sort_bus_line(List_Bus_Line l)
{
	List_Bus_Line i, j, selected;
	for (i=l; i!=NULL; i=_get_next_node(i))
	{
		selected = i;
		for (j=_get_next_node(i); j!= NULL; j=_get_next_node(j))
		{
			Bus_Line_Object* obj_j = _get_node(j);
			Bus_Line_Object* obj_selected = _get_node(selected);
			int price_j = get_maintenance_price(obj_j);
			int price_selected = get_maintenance_price(obj_selected);
			if (price_j > price_selected || (price_j == price_selected && _is_date_more_recent(obj_j, obj_selected))) selected = j;
		}
		if (selected != i) swap_node(i, selected);
	}
}
