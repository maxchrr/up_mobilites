#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "utils.h"

List create_random(int s)
{
	List l;
	init_list(&l);
	srand(time(NULL)); // To ensure pseudo-random generation
	for (int i=0; i<s; ++i)
		l = insert_at_tail(l, rand()%100);
	return l;
}
