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

int* list_to_array(List l)
{
	int len = length(l);
	int* a = calloc(len, sizeof(int));
	for (int i=0; i<len; ++i)
	{
		a[i] = *get_node(l);
		l = get_next_node(l);
	}
	return a;
}

void print_array(int* a, int s) {
	for (int i=0; i<s; ++i)
		printf("%d ",a[i]);
	printf("\n");
}
