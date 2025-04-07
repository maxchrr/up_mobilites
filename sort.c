#include <stdio.h>
#include "list.h"
#include "sort.h"

void selection_sort(List_Bus_Line l)
{
	struct Node* i;
	struct Node* j;
	struct Node* j_min;
	struct Node* temp = _get_last_node(l);
	for (i=l; i!=temp; i=_get_next_node(i))
	{
		j_min = i;
		for (j=i; j!=temp; j=_get_next_node(j))
		{
			if (_get_node(j) < _get_node(j_min))
				j_min = j; 
		}
		if (_get_node(temp) < _get_node(j_min))
			j_min = temp;
	}
}
