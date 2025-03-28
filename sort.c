#include <stdio.h>
#include "sort.h"

void selection_sort(int* a, int s)
{
	int i, j, j_min;
	for (i=0; i<s-1; ++i)
	{
		j_min = i;
		for (j=i+1;j<s; ++j)
		{
			if (a[j] < a[j_min])
				j_min =j;
		}
		int temp = a[i];
		a[i] = a[j_min];
		a[j_min] = temp;
	}
}
