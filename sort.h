#ifndef SORT_H_
#define SORT_H_

#include <stdbool.h>

bool _is_date_more_recent(Bus_Line_Object* bl1, Bus_Line_Object* bl2);
bool _is_price_higher(Bus_Line_Object* bl1, Bus_Line_Object* bl2);
void sort_bus_line(List_Bus_Line l);

#endif // SORT_H_
