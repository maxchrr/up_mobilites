#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "utils.h"


struct Bus_Stop {
	int id;                             // Identifiant unique pour l'arrêt de bus (non liée à une ligne de bus)
	char name[30];                      // Nom de l'arrêt (pour l'affichage)
	int posx, posy;                   // Coordonées sur le plan
	int maint_price;              // Prix de maintenance (keuro)
	struct Date last_maint_date;  // Date de la dernière maintenance
};

struct Bus_Route
{
	int bus_line_id;             // Identifiant de la ligne de bus entrante
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

struct Bus_Route* create_br(int bus_line_id, struct Bus_Stop* departure, struct Bus_Stop* arrival, int distance_due, int time_due)
{
	struct Bus_Route* new_br = malloc(sizeof(struct Bus_Route));
	if (!new_br)
	{
		fprintf(stderr, "Memory allocation failed for BUS_ROUTE\n");
		return NULL;
	}
	memset(new_br, 0, sizeof(struct Bus_Route));
	new_br->bus_line_id = bus_line_id;
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

struct Bus_Line* create_bl(struct Bus_Route* br)
{
	struct Bus_Line* new_bl = malloc(sizeof(struct Bus_Line));
	new_bl->stop = 0;
	new_bl->route = 1;
	new_bl->u.br = *br;
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
			"%*sarrêt %d \"%s\" (%d,%d)\n",
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
			bl->u.br.bus_line_id,
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

int br_getbus_line_id(struct Bus_Route* br)
{
	return br->bus_line_id;
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

int br_gettime_tue(struct Bus_Route* br)
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
