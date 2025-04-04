#include <stdio.h>
#include "list.h"
#include "sort.h"

void selection_sort(List_Bus_Line l)
{
	struct Node* i;
	struct Node* j;
	struct Node* j_min;
	struct Node* temp = get_last_node(l);
	for (i=l; i!=temp; i=get_next_node(i))
	{
		j_min = i;
		for (j=i; j!=temp; j=get_next_node(j))
		{
			if (get_node(j) < get_node(j_min))
				j_min = j; 
		}
		if (get_node(temp) < get_node(j_min))
			j_min = temp;
	}
}
