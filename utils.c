#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "utils.h"

int rand_range(int min, int max)
{
	static int seeded = 0;
	if (!seeded)
	{
		srand(time(NULL) + clock()); // To ensure pseudo-random generation
		seeded = 1;
	}
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

List_Bus_Line create_random(int sz)
{
	List_Bus_Line l;
	init_list(&l);
	srand(time(NULL)); // To ensure pseudo-random generation
	for (int i=0; i<sz; ++i)
		l = insert_at_tail(l, NULL);
	return l;
}
