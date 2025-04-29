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

#define MAX_NAME_LEN		32

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
	memset(new_bs, 0, sizeof(BusStation));
	new_bs->id = id;
	strncpy(new_bs->name, name, sizeof(new_bs->name));
	new_bs->posx = posx;
	new_bs->posy = posy;
	new_bs->maint_price = rand_range(10,100);
	Date maint_date = rand_date(2018, 2024);
	new_bs->last_maint_date = maint_date;
	return new_bs;
}

void print_bs(const BusStation* bs, int indent)
{
	if (!bs)
	{
		fprintf(stdout, "[NULL station]\n");
		return;
	}
	fprintf(
		stdout,
		"Arrêt %d \"%s\" (%d,%d)\n",
		bs->id,
		bs->name,
		bs->posx,
		bs->posy
	);
	fprintf(
		stdout,
		"%*sDernière maintenance le %02d/%02d/%04d (%dk€)\n",
		indent+4,"",
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
	int dx = bs_getposx(arrival)-bs_getposx(departure);
	int dy = bs_getposy(arrival)-bs_getposy(departure);
	int due = hypot(dx,dy); // Distance entre deux points, avec leurs coordonnées cartésiennes
	new_br->distance_due = due;
	new_br->time_due = due;
	return new_br;
}

void print_br(const BusRoute* br, int indent)
{
	if (!br)
	{
		fprintf(stdout, "[NULL route]\n");
		return;
	}
	fprintf(stdout, "*****************************************\n");
	fprintf(stdout, "*\t\tLigne %d\t\t*\n", br->bl_id);
	fprintf(stdout, "*****************************************\n");
	fprintf(
		stdout,
		"Parcours > distance %dm / temps %ds\n",
		br->distance_due,
		br->time_due
	);
	fprintf(stdout, "%*s-- De : ", indent+2,"");
	print_bs(br->departure, 0);
	fprintf(stdout, "%*s-- À : ", indent+2,"");
	print_bs(br->arrival, 0);
}

void destroy_br(BusRoute* br)
{
	free(br);
}

BusEntity* open_entity(int is_station, void* data)
{
	BusEntity* new_entity = malloc(sizeof(BusEntity));
	if (!new_entity)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	if (is_station)
	{
		new_entity->station = 1;
		new_entity->route = 0;
		new_entity->bs = (BusStation*)data;
	}
	else
	{
		new_entity->station = 0;
		new_entity->route = 1;
		new_entity->br = (BusRoute*)data;
	}
	return new_entity;
}

void print_entity(const BusEntity* obj, int indent)
{
	if (obj == NULL)
	{
		fprintf(
			stdout,
			"%*sEntité inexistante ou non allouée\n",
			indent,""
		);
		return;
	}
	EntityType type = gettype(obj);
	if (type == INVALID)
	{
		fprintf(
			stdout,
			"%*sType inconnue ?\n",
			indent,""
		);
		return;
	}
	/*if (type == STATION)
	{
		print_bs(obj->bs, indent);
	}*/
	if (type == ROUTE)
	{
		print_br(obj->br, indent);
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
