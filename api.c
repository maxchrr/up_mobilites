/*
 * Public API
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "utils.h"
#include "list.h"

struct Bus_Stop {
	int id;                       // Identifiant unique pour l'arrêt de bus (non liée à une ligne de bus)
	char name[30];                // Nom de l'arrêt (pour l'affichage)
	int posx, posy;               // Coordonées sur le plan
	int maint_price;              // Prix de maintenance (keuro)
	struct Date last_maint_date;  // Date de la dernière maintenance
};

struct Bus_Route
{
	int bl_id;                   // Identifiant de la ligne de bus entrante
	struct Bus_Stop* departure;  // Pointeur sur l'arrêt entrant
	struct Bus_Stop* arrival;    // Pointeur sur l'arrêt sortant
	int distance_due;            // Coût en distance (mètres)
	int time_due;                // Coût en temps de parcours (secondes)
};

struct Bus_Line
{
	unsigned stop : 1;
	unsigned route : 1;
	union
	{
		struct Bus_Stop bs;
		struct Bus_Route br;
	} u;
};

struct Bus
{
	int id;
	int posx, posy;
	int bl_id;
	List pos_in_list;
	Bus_Direction direction;
};

struct Bus_Stop* create_bs(int id, char* name, int posx, int posy)
{
	struct Bus_Stop* new_bs = malloc(sizeof(struct Bus_Stop));
	if (!new_bs)
	{
		fprintf(stderr, "Memory allocation failed for BUS_STOP\n");
		return NULL;
	}
	memset(new_bs, 0, sizeof(struct Bus_Stop));
	new_bs->id = id;
	strncpy(new_bs->name, name, sizeof(new_bs->name));
	new_bs->posx = posx;
	new_bs->posy = posy;
	new_bs->maint_price = rand_range(10,100);
	struct Date maint_date = rand_date(2018, 2024);
	new_bs->last_maint_date = maint_date;
	return new_bs;
}

void free_bs(struct Bus_Stop* bs)
{
	free(bs);
}

struct Bus_Route* create_br(int bl_id, struct Bus_Stop* departure, struct Bus_Stop* arrival, int distance_due, int time_due)
{
	struct Bus_Route* new_br = malloc(sizeof(struct Bus_Route));
	if (!new_br)
	{
		fprintf(stderr, "Memory allocation failed for BUS_ROUTE\n");
		return NULL;
	}
	memset(new_br, 0, sizeof(struct Bus_Route));
	new_br->bl_id = bl_id;
	new_br->departure = departure;
	new_br->arrival = arrival;
	new_br->distance_due = distance_due;
	new_br->time_due = time_due;
	return new_br;
}

void free_br(struct Bus_Route* br)
{
	free(br);
}

struct Bus_Line create_bl(int is_stop, void* data)
{
	struct Bus_Line new_bl;
	if (is_stop)
	{
		new_bl.stop = 1;
		memcpy(&new_bl.u.bs, data, sizeof(struct Bus_Stop));
	}
	else
	{
		new_bl.route = 1;
		memcpy(&new_bl.u.br, data, sizeof(struct Bus_Route));
	}
	return new_bl;
}

void print_bl(struct Bus_Line* bl, int indent)
{
	if (bl == NULL)
	{
		fprintf(
			stdout,
			"%*sLigne inexistante ou non allouée\n",
			indent,""
		);
		return;
	}
	if (bl->stop && !bl->route)
	{
		fprintf(
			stdout,
			"%*sArrêt %d \"%s\" (%d,%d)\n",
			indent, "",
			bl->u.bs.id,
			bl->u.bs.name,
			bl->u.bs.posx,
			bl->u.bs.posy
		);
		fprintf(
			stdout,
			"%*sDernière maintenance le %02d/%02d/%04d (%dk€)\n",
			indent+4,"",
			bl->u.bs.last_maint_date.day,
			bl->u.bs.last_maint_date.month,
			bl->u.bs.last_maint_date.year,
			bl->u.bs.maint_price
		);
	}
	else
	{
		fprintf(
			stdout,
			"%*sLigne %d : distance %dm / temps %ds\n",
			indent,"",
			bl->u.br.bl_id,
			bl->u.br.distance_due,
			bl->u.br.time_due
		);
		fprintf(stdout, "%*s-- De : ", indent+2,"");
		struct Bus_Line temp_departure;
		temp_departure.stop = 1;
		temp_departure.route = 0;
		temp_departure.u.bs = *bl->u.br.departure;
		print_bl(&temp_departure, 0);
		fprintf(stdout, "%*s-- À : ", indent+2,"");
		struct Bus_Line temp_arrival;
		temp_arrival.stop = 1;
		temp_arrival.route = 0;
		temp_arrival.u.bs = *bl->u.br.arrival;
		print_bl(&temp_arrival, 0);
	}
}

void free_bl(struct Bus_Line* bl)
{
	free(bl);
}

struct Bus* create_bus(int id, List start)
{
	struct Bus* new_bus = malloc(sizeof(struct Bus));
	new_bus->id = id;
	bus_setbus_on_bl(new_bus, start, DEP_TO_ARR);
	new_bus->bl_id = br_getbl_id((struct Bus_Route*) _get_node(br_getnext(start)));
	return new_bus;
}

void free_bus(struct Bus* bus)
{
	free(bus);
}

void print_list_bl(List list_bl)
{
	_print_list(list_bl);
	fprintf(stdout, "Fin de la ligne\n");
}

int bs_getid(struct Bus_Stop* bs)
{
	return bs->id;
}

char* bs_getname(struct Bus_Stop* bs)
{
	return bs->name;
}

int bs_getposx(struct Bus_Stop* bs)
{
	return bs->posx;
}

int bs_getposy(struct Bus_Stop* bs)
{
	return bs->posy;
}

int bs_getmaint_price(struct Bus_Stop* bs)
{
	return bs->maint_price;
}

struct Date bs_getlast_maint_date(struct Bus_Stop* bs)
{
	return bs->last_maint_date;
}

void bs_setmaint_price(struct Bus_Stop* bs, int value)
{
	bs->maint_price = value;
}

void bs_setlast_maint_date(struct Bus_Stop* bs, struct Date date)
{
	bs->last_maint_date = date;
}

int br_getbl_id(struct Bus_Route* br)
{
	return br->bl_id;
}

struct Bus_Stop* br_getdeparture(struct Bus_Route* br)
{
	return br->departure;
}

struct Bus_Stop* br_getarrival(struct Bus_Route* br)
{
	return br->arrival;
}

int br_getdistance_due(struct Bus_Route* br)
{
	return br->distance_due;
}

int br_gettime_due(struct Bus_Route* br)
{
	return br->time_due;
}

Bus_Line_Type bl_gettype(struct Bus_Line* bl)
{
	if (bl == NULL)                   return BUS_LINE_INVALID;
	if (bl->stop && !bl->route)       return BUS_LINE_STOP;
	else if (!bl->stop && bl->route)  return BUS_LINE_ROUTE;
	else                              return BUS_LINE_INVALID;
}

int bus_getid(struct Bus* bus)
{
	return bus->id;
}

int bus_getposx(struct Bus* bus)
{
	return bus->posx;
}

int bus_getposy(struct Bus* bus)
{
	return bus->posy;
}

int bus_getbl_id(struct Bus* bus)
{
	return bus->bl_id;
}

List bus_getpos_in_list(struct Bus* bus)
{
	return bus->pos_in_list;
}

Bus_Direction bus_getdirection(struct Bus* bus)
{
	return bus->direction;
}

void bus_setposx(struct Bus* bus, int posx)
{
	bus->posx = posx;
}

void bus_setposy(struct Bus* bus, int posy)
{
	bus->posy = posy;
}

void bus_setbl_id(struct Bus* bus, int bl_id)
{
	bus->bl_id = bl_id;
}

void bus_setpos_in_list(struct Bus* bus, List bl)
{
	bus->pos_in_list = bl;
}

void bus_setdirection(struct Bus* bus, Bus_Direction direction)
{
	bus->direction = direction;
}

void bus_setbus_on_bl(struct Bus* bus, List bl, Bus_Direction direction)
{
	bus_setpos_in_list(bus, bl);
	bus_setdirection(bus, direction);
	bus_setposx(bus, list_getposx(bl));
	bus_setposy(bus, list_getposy(bl));
	fprintf(
		stdout,
		"Bus %d en (%d,%d) sur Ligne %d / Arrêt %s\n",
		bus_getid(bus),
		bus_getposx(bus),
		bus_getposy(bus),
		bus_getbl_id(bus),
		bs_getname((struct Bus_Stop*) _get_node(bl))
	);
}

struct Node* bs_getnext(List l)
{
	if (is_empty(l))
		return NULL;
	if (bl_gettype(_get_node(l)) == BUS_LINE_STOP)
	{
		if (is_empty(_get_next_node(l)))
			return NULL; // Terminal station
		else
			return _get_next_node(_get_next_node(l));
			// Iter over two nodes, next one is BUS_ROUTE, then BUS_STOP
	}
	else
		return _get_next_node(l); // Next node is BUS_STOP
}

struct Node* bs_getprev(List l)
{
	if (is_empty(l))
		return NULL;
	if (bl_gettype(_get_node(l)) == BUS_LINE_ROUTE)
	{
		if (is_empty(_get_prev_node(l)))
			return NULL; // Terminal station
		else
			return _get_prev_node(_get_prev_node(l));
			// Iter backwards over two nodes, previous one is BUS_ROUTE, then BUS_STOP
	}
	else
		return _get_prev_node(l); // Previous node is BUS_STOP
}

struct Node* br_getnext(List l)
{
	if (is_empty(l))
		return NULL;
	if (bl_gettype(_get_node(l)) == BUS_LINE_ROUTE)
		return _get_next_node(l); // Next node is BUS_ROUTE
	return l;
}

struct Node* br_getprev(List l)
{
	if (is_empty(l))
		return NULL;
	if (bl_gettype(_get_node(l)) == BUS_LINE_ROUTE)
		return _get_prev_node(l); // Previous node is BUS_ROUTE
	return l;
}

int list_getposx(List l)
{
	return bs_getposx((struct Bus_Stop*) _get_node(l));
}

int list_getposy(List l)
{
	return bs_getposy((struct Bus_Stop*) _get_node(l));
}
