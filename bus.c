/*
 * Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "api.h"
#include "list.h"
#include "bus.h"
#include "loader.h"

Bus* init_bus(int id, BusLine bl)
{
	Bus* new_bus = malloc(sizeof(Bus));
	new_bus->id = id;
	bus_departure(new_bus, bl, DEP_TO_ARR);
	new_bus->speed = 100.0f;
	new_bus->stop_time = 0.0f;
	new_bus->is_stopping = false;
	return new_bus;
}

void print_bus(const Bus* bus)
{
	fprintf(
		stdout,
		"[BUS #%d] ROUTE #%d \"%s\" (%d,%d) --> %s\n",
		bus_getid(bus),
		bus_getbl_id(bus),
		bs_getname(_get_node(bus->bl)->bs),
		bus_getposx(bus),
		bus_getposy(bus),
		(bus_getdirection(bus) == DEP_TO_ARR) ? "ARR" : "DEP"
	);
}

void destroy_bus(Bus* bus)
{
	//destroy_list(bus->bl);
	free(bus);
}

int bus_getid(const Bus* bus)
{
	return bus->id;
}

int bus_getposx(const Bus* bus)
{
	return bus->posx;
}

int bus_getposy(const Bus* bus)
{
	return bus->posy;
}

int bus_getbl_id(const Bus* bus)
{
	return bus->bl_id;
}

BusLine bus_getbl(const Bus* bus)
{
	return bus->bl;
}

BusDirection bus_getdirection(const Bus* bus)
{
	return bus->direction;
}

float bus_getspeed(const Bus* bus)
{
	return bus->speed;
}

float bus_getstop_time(const Bus* bus)
{
	return bus->stop_time;
}

bool bus_getis_stopping(const Bus* bus)
{
	return bus->is_stopping;
}

int bl_getcurrent_posx(BusLine l)
{
	return bs_getposx(_get_node(l)->bs);
}

int bl_getcurrent_posy(BusLine l)
{
	return bs_getposy(_get_node(l)->bs);
}

BusLine bl_getnext_bs(BusLine l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) == STATION)
	{
		if (is_empty(_get_next_node(l))) return NULL; // Terminus
		else return _get_next_node(_get_next_node(l)); // Le suivant est une route, donc après une station
	}
	else return _get_next_node(l); // Le suivant est une station*/
}

BusLine bl_getprev_bs(BusLine l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) == STATION)
	{
		if (is_empty(_get_prev_node(l))) return NULL; // Terminus
		else return _get_prev_node(_get_prev_node(l)); // Le précédent est une route, donc avant une station
	}
	else return _get_prev_node(l); // Le précédent est une station
}

BusLine bl_getnext_br(BusLine l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) != STATION)
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return NULL;
	}
	else
		return _get_next_node(l); // Le suivant est une route
}

BusLine bl_getprev_br(BusLine l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) != STATION)
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return NULL;
	}
	else
		return _get_prev_node(l); // Le précédent est une route
}

void bus_setposx(Bus* bus, int value)
{
	bus->posx = value;
}

void bus_setposy(Bus* bus, int value)
{
	bus->posy = value;
}

void bus_setbl_id(Bus* bus, int value)
{
	bus->bl_id = value;
}

void bus_setbl(Bus* bus, BusLine bl)
{
	bus->bl = bl;
}

void bus_setdirection(Bus* bus, BusDirection value)
{
	bus->direction = value;
}

void bus_setspeed(Bus* bus, float value)
{
	bus->speed = value;
}

void bus_setstop_time(Bus* bus, float value)
{
	bus->stop_time = value;
}

void bus_setis_stopping(Bus* bus, bool value)
{
	bus->is_stopping = value;
}

void bus_departure(Bus* bus, BusLine bl, BusDirection direction)
{
	BusEntity* next = _get_node(_get_next_node(bl));
	bus_setbl_id(bus, (gettype(next) == ROUTE) ? br_getbl_id(next->br) : -1);
	bus_setbl(bus, bl);
	bus_setdirection(bus, direction);
	bus_setposx(bus, bl_getcurrent_posx(bl));
	bus_setposy(bus, bl_getcurrent_posy(bl));
	print_bus(bus);
}

void bus_travel(Bus* bus, BusDirection direction, int* incx, int* incy, float delta, double time)
{
	BusLine current;
	if (bus_getis_stopping(bus))
	{
		if (time-bus_getstop_time(bus) >= 2.0f)
			bus_setis_stopping(bus, false);
		return;
	}
	current = (direction == DEP_TO_ARR) ? bl_getnext_bs(bus_getbl(bus))
	                                    : bl_getprev_bs(bus_getbl(bus));
	if (is_empty(current))
	{
		bus_setdirection(bus, (direction == DEP_TO_ARR) ? ARR_TO_DEP : DEP_TO_ARR); // Changement de direction automatique au terminus
		return;
	}
	int xd = bus_getposx(bus), yd = bus_getposy(bus);
	int xa = bl_getcurrent_posx(current), ya = bl_getcurrent_posy(current);
	float dx = xa-xd, dy = ya-yd;
	float dist = sqrtf(dx*dx + dy*dy);
	if (dist < 1.0f) // Arrêt si proche
	{
		print_bus(bus);
		bus_setbl(bus, current);
		bus_setstop_time(bus, time);
		bus_setis_stopping(bus, true);
		return;
	}
	float speed = bus_getspeed(bus);
	float move = speed*delta;
	dx *= move/dist; dy *= move/dist;
	bus_setposx(bus, xd+dx); bus_setposy(bus, yd+dy);
	// Ces valeurs accumulent les déplacements pour éviter les pertes dues au cast
	static float accumx = 0.0f;
	static float accumy = 0.0f;
	accumx += dx; accumy += dy;
	*incx = (int)accumx; *incy = (int)accumy;
	accumx -= *incx; accumy -= *incy;
}

BusLine bus_line_create(int id, const char *name) {
    BusLine bl;
    init_list(&bl);
    return bl;
}

void bus_line_add_station(BusLine *line, BusStation *station) {
    BusEntity *ent = open_entity(STATION, station);
    *line = insert_at_tail(*line, ent);
}

BusLine create_from_existente(BusLine line1, BusLine line2) {
    if (!line1 || !line2) return NULL;

    // Création de la nouvelle ligne
    BusLine new_line = bus_line_create(-1, "LigneFusion");
    if (!new_line) return NULL;

    // 1) Première station de line1
    Node *start_node = line1;                         // head de la liste
    BusEntity *e1   = (BusEntity*)start_node->data;   // on récupère l'entité
    BusStation *start = e1->bs;                       // on prend le pointeur bs
    bus_line_add_station(&new_line, start);

    // 2) Dernière station de line2
    Node *end_node = line2;
    while (end_node->next) end_node = end_node->next;
    BusEntity *e2 = (BusEntity*)end_node->data;
    BusStation *end = e2->bs;
    bus_line_add_station(&new_line, end);

    return new_line;
}