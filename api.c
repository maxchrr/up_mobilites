/*
 * Public API
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "api.h"

struct BusStation
{
	int id;                   // Identifiant unique pour l'arrêt de bus (non liée à une ligne de bus)
	char name[MAX_NAME_LEN];  // Nom de l'arrêt (pour l'affichage)
	int posx, posy;           // Coordonées sur le plan
	int maint_price;          // Prix de maintenance (keuro)
	Date last_maint_date;     // Date de la dernière maintenance
};

struct BusRoute
{
	int bl_id;              // Identifiant de la ligne de bus entrante
	BusStation* departure;  // Pointeur sur l'arrêt entrant
	BusStation* arrival;    // Pointeur sur l'arrêt sortant
	int distance_due;       // Coût en distance (mètres)
	int time_due;           // Coût en temps de parcours (secondes)
};

BusStation* create_bs(int id, const char* name, int posx, int posy)
{
	BusStation* new_bs = malloc(sizeof(BusStation));
	if (!new_bs)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	new_bs->id = id;
	strncpy(new_bs->name, name, sizeof(new_bs->name));
	new_bs->name[MAX_NAME_LEN - 1] = '\0';  // Suppresion du surplus de caractère
	new_bs->posx = posx;
	new_bs->posy = posy;
	new_bs->maint_price = rand_range(10,100);
	Date maint_date = rand_date(2018, 2024);
	new_bs->last_maint_date = maint_date;
	return new_bs;
}

void print_bs(const BusStation* bs)
{
	if (!bs)
	{
		fprintf(stdout, "[NULL STATION]\n");
		return;
	}
	fprintf(
		stdout,
		"[STATION #%d] \"%s\" (%d,%d)\n",
		bs->id,
		bs->name,
		bs->posx,
		bs->posy
	);
	fprintf(
		stdout,
		"  --> [MAINT] %02d/%02d/%04d (%dk€)\n",
		bs->last_maint_date.day,
		bs->last_maint_date.month,
		bs->last_maint_date.year,
		bs->maint_price
	);
}

void destroy_bs(BusStation* bs)
{
	free(bs);
}

BusRoute* create_br(int bl_id, BusStation* departure, BusStation* arrival)
{
	BusRoute* new_br = malloc(sizeof(BusRoute));
	if (!new_br)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	new_br->bl_id = bl_id;
	new_br->departure = departure;
	new_br->arrival = arrival;
	if (!departure || !arrival)
	{
		fprintf(stderr, "Null pointer in create_br\n");
		free(new_br);
		return NULL;
	}
	int dx = bs_getposx(arrival)-bs_getposx(departure);
	int dy = bs_getposy(arrival)-bs_getposy(departure);
	int due = (int)round(hypot(dx,dy));  // Distance entre deux points, avec leurs coordonnées cartésiennes
	new_br->distance_due = due;
	new_br->time_due = (int)due / 10.0;  // Coefficient arbitraire - vitesse de 10 m/s
	return new_br;
}

void print_br(const BusRoute* br)
{
	if (!br)
	{
		fprintf(stdout, "[NULL ROUTE]\n");
		return;
	}
	fprintf(
		stdout,
		"[ROUTE #%d] %s -> %s (%dm / %ds)\n",
		br->bl_id,
		bs_getname(br->departure),
		bs_getname(br->arrival),
		br->distance_due,
		br->time_due
	);
}

void destroy_br(BusRoute* br)
{
	free(br);
}

BusEntity* open_entity(EntityType type, void* e)
{
	BusEntity* new_entity = malloc(sizeof(BusEntity));
	if (!new_entity)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	if (type == STATION)
	{
		new_entity->station = (type == STATION);
		new_entity->route = (type == ROUTE);
		new_entity->bs = (BusStation*)e;
	}
	else if (type == ROUTE)
	{
		new_entity->station = (type == STATION);
		new_entity->route = (type == ROUTE);
		new_entity->br = (BusRoute*)e;
	}
	return new_entity;
}

void print_entity(const BusEntity* obj)
{
	if (obj == NULL)
	{
		fprintf(stdout, "[NULL ENTITY]\n");
		return;
	}
	EntityType type = gettype(obj);
	if (type == INVALID)
	{
		fprintf(stdout, "[INVALID ENTITY TYPE]\n");
		return;
	}
	if (type == STATION)
	{
		print_bs(obj->bs);
	}
	if (type == ROUTE)
	{
		print_br(obj->br);
	}
}

void close_entity(BusEntity* obj)
{
	if (obj != NULL)
	{
		if (obj->station)	destroy_bs(obj->bs);
		else if (obj->route)	destroy_br(obj->br);
		free(obj);
	}
}

int bs_getid(const BusStation* bs)
{
	if (!bs) return -1;
	return bs->id;
}

const char* bs_getname(const BusStation* bs)
{
	return bs->name;
}

int bs_getposx(const BusStation* bs)
{
	return bs->posx;
}

int bs_getposy(const BusStation* bs)
{
	return bs->posy;
}

int bs_getmaint_price(const BusStation* bs)
{
	return bs->maint_price;
}

Date bs_getlast_maint_date(const BusStation* bs)
{
	return bs->last_maint_date;
}

void bs_setmaint_price(BusStation* bs, int value)
{
	bs->maint_price = value;
}

void bs_setlast_maint_date(BusStation* bs, Date date)
{
	bs->last_maint_date = date;
}

int br_getbl_id(const BusRoute* br)
{
	return br->bl_id;
}

BusStation* br_getdeparture(const BusRoute* br)
{
	return br->departure;
}

BusStation* br_getarrival(const BusRoute* br)
{
	return br->arrival;
}

int br_getdistance_due(const BusRoute* br)
{
	return br->distance_due;
}

int br_gettime_due(const BusRoute* br)
{
	return br->time_due;
}

EntityType gettype(const BusEntity* obj)
{
	if (obj->station && !obj->route)
	{
		return STATION;
	}
	else if (!obj->station && obj->route)
	{
		return ROUTE;
	}
	else	return INVALID;
}
