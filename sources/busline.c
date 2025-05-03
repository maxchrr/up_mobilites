/*
 * Ligne de Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "list.h"
#include "loader.h"
#include "ui.h"
#include "busline.h"

BusLine create_bl(int id)
{
	BusLine new_bl;
	new_bl.id = id;
	return new_bl;
}

void init_bl(BusLine* bl, const char* path)
{
	bl->list = read_file(bl->id, path);
	if (!bl->list)
	{
		fprintf(stderr, "Erreur lors du chargement du fichier\n");
		destroy_list(bl->list);
		exit(EXIT_FAILURE);
	}
	unsigned capacity = 10;  // Valeur assez grande pour éviter le redimensionnement
	bl->bus_arr = calloc(capacity, sizeof(Bus*));
	if (!bl->bus_arr)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	bl->bus_count = 0;
	bl->arr_capacity = capacity;
	bl->color = random_color();
}

void print_bl(BusLine bl)
{
	print_list(bl.list);
}

void destroy_bl(BusLine bl)
{
	if (!bl.list) return;
	destroy_list(bl.list);
	bl.list = NULL;
	bl.id = 0;
	bl.color = (Color){0};  // Pour nettoyage visuel
}

int bl_getcurrent_posx(List bl)
{
	return bs_getposx(list_getnode(bl)->bs);
}

int bl_getcurrent_posy(List bl)
{
	return bs_getposy(list_getnode(bl)->bs);
}

List bl_getnext_bs(List l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) == STATION)
	{
		if (list_is_empty(list_getnext_node(l)))
			return NULL; // Terminus
		else
			return list_getnext_node(list_getnext_node(l)); // Le suivant est une route, donc après une station
	}
	else if (gettype(list_getnode(l)) == ROUTE)
		return list_getnext_node(l); // Le suivant est une station
	else
		return NULL;
}

List bl_getprev_bs(List l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) == STATION)
	{
		if (list_is_empty(list_getprev_node(l)))
			return NULL; // Terminus
		else
			return list_getprev_node(list_getprev_node(l)); // Le précédent est une route, donc avant une station
	}
	else if (gettype(list_getnode(l)) == ROUTE)
		return list_getprev_node(l); // Le précédent est une station
	else
		return NULL;
}

List bl_getnext_br(List l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) != STATION)
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return NULL;
	}
	else if (gettype(list_getnode(l)) != ROUTE)
		return list_getnext_node(l); // Le suivant est une route
	else
		return NULL;
}

List bl_getprev_br(List l)
{
	if (list_is_empty(l)) return NULL;
	if (gettype(list_getnode(l)) != STATION)
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return NULL;
	}
	else if (gettype(list_getnode(l)) != ROUTE)
		return list_getprev_node(l); // Le précédent est une route
	else
		return NULL;
}

void bl_add_bus(BusLine* bl, Bus* bus)
{
	if (bl->bus_count >= bl->arr_capacity)
	{
		bl->arr_capacity *= 2;
		Bus** new_arr = realloc(bl->bus_arr, sizeof(Bus*) * bl->arr_capacity);
		if (!new_arr)
		{
			fprintf(stderr, "Memory allocation failed\n");
			return;
		}
		bl->bus_arr = new_arr;
	}
	bl->bus_arr[bl->bus_count++] = bus;
}

void bl_remove_bus(BusLine* bl, int bus_id)
{
	if (bl->bus_count == 0) return;
	bool found = false;
	// Chercher le bus
	for (unsigned i=0; i<bl->bus_count; ++i)
	{
		if (!found && bl->bus_arr[i]->id == bus_id)
		{
			free(bl->bus_arr[i]);
			found = true;
		}
		if (found && i < bl->bus_count-1)
			bl->bus_arr[i] = bl->bus_arr[i+1];
	}
	if (found)
		bl->bus_count--;
}

List bl_remove(List l)
{
	l = delete_at_tail(l);  // Dernière station
	l = delete_at_tail(l);  // Dernière route
	return l;
}

List bl_concat(List l1, List l2)
{
	BusStation* last_station = list_getnode(list_getlast_node(l1))->bs;
	BusRoute* last_route = list_getnode(list_getprev_node(list_getlast_node(l1)))->br;
	BusStation* new_first_station = list_getnode(list_getfirst_node(l2))->bs;
	last_route = create_br(br_getbl_id(last_route), last_station, new_first_station);
	l1 = insert_at_tail(l1, open_entity(ROUTE, last_route));
	l1 = list_append(l1, l2);
	return l1;
}
