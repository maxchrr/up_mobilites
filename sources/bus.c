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
#include "raylib.h"

BusPtr init_bus(int id, List bl)
{
	BusPtr new_bus = malloc(sizeof(Bus));
	new_bus->id = id;
	bus_departure(new_bus, bl, DEP_TO_ARR);
	Node* next = _get_next_node(bl);
	if (is_empty(next) || gettype(_get_node(next)) != ROUTE)
	{
		fprintf(stderr, "Mauvais type\n");
		free(new_bus);
		return NULL;
	}
	new_bus->bl_id = br_getbl_id(_get_node(next)->br);
	new_bus->speed = 100.0f;
	new_bus->stop_time = 0.0f;
	new_bus->is_stopping = false;
	return new_bus;
}

void print_bus(const BusPtr bus)
{
	fprintf(
		stdout,
		"Bus %d sur ligne %d départ %s (%d,%d)\n",
		bus_getid(bus),
		bus_getbl_id(bus),
		bs_getname(_get_node(bus->bl)->bs),
		bus_getposx(bus),
		bus_getposy(bus)
	);
}

void destroy_bus(BusPtr bus)
{
	//destroy_list(bus->bl);
	free(bus);
}

int bus_getid(const BusPtr bus)
{
	return bus->id;
}

int bus_getposx(const BusPtr bus)
{
	return bus->posx;
}

int bus_getposy(const BusPtr bus)
{
	return bus->posy;
}

int bus_getbl_id(const BusPtr bus)
{
	return bus->bl_id;
}

List bus_getbl(const BusPtr bus)
{
	return bus->bl;
}

BusDirection bus_getdirection(const BusPtr bus)
{
	return bus->direction;
}

float bus_getspeed(const BusPtr bus)
{
	return bus->speed;
}

float bus_getstop_time(const BusPtr bus)
{
	return bus->stop_time;
}

bool bus_getis_stopping(const BusPtr bus)
{
	return bus->is_stopping;
}

int bl_getposx(List l)
{
	return bs_getposx(_get_node(l)->bs);
}

int bl_getposy(List l)
{
	return bs_getposy(_get_node(l)->bs);
}

List bl_getnext_bs(List l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) == STATION)
	{
		if (is_empty(_get_next_node(l))) return NULL; // Terminus
		else return _get_next_node(_get_next_node(l)); // Le suivant est une route, donc après une station
	}
	else return _get_next_node(l); // Le suivant est une station*/
}

List bl_getprev_bs(List l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) == STATION)
	{
		if (is_empty(_get_prev_node(l))) return NULL; // Terminus
		else return _get_prev_node(_get_prev_node(l)); // Le précédent est une route, donc avant une station
	}
	else return _get_prev_node(l); // Le précédent est une station
}

List bl_getnext_br(List l)
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

List bl_getprev_br(List l)
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

void bus_setposx(BusPtr bus, int value)
{
	bus->posx = value;
}

void bus_setposy(BusPtr bus, int value)
{
	bus->posy = value;
}

void bus_setbl_id(BusPtr bus, int value)
{
	bus->bl_id = value;
}

void bus_setbl(BusPtr bus, List bl)
{
	bus->bl = bl;
}

void bus_setdirection(BusPtr bus, BusDirection value)
{
	bus->direction = value;
}

void bus_setspeed(BusPtr bus, float value)
{
	bus->speed = value;
}

void bus_setstop_time(BusPtr bus, float value)
{
	bus->stop_time = value;
}

void bus_setis_stopping(BusPtr bus, bool value)
{
	bus->is_stopping = value;
}

void bus_departure(BusPtr bus, List bl, BusDirection direction)
{
	bus_setbl(bus, bl);
	bus_setdirection(bus, direction);
	bus_setposx(bus, bl_getposx(bl));
	bus_setposy(bus, bl_getposy(bl));
	print_bus(bus);
}

void bus_travel(BusPtr bus, BusDirection direction, int* incx, int* incy)
{
	List current;
	if (bus_getis_stopping(bus))
	{
		if (GetTime()-bus_getstop_time(bus) >= 2.0f)
			bus_setis_stopping(bus, false);
		return;
	}
	current = (direction == DEP_TO_ARR) ? bl_getnext_bs(bus_getbl(bus))
	                                    : bl_getprev_bs(bus_getbl(bus));
	if (is_empty(current)) return;
	int xd = bus_getposx(bus);
	int yd = bus_getposy(bus);
	int xa = bl_getposx(current);
	int ya = bl_getposy(current);
	float dx = xa-xd;
	float dy = ya-yd;
	float dist = sqrtf(dx*dx + dy*dy);
	if (dist < 1.0f) // Arrêt si proche
	{
		print_bus(bus);
		bus_setbl(bus, current);
		bus_setstop_time(bus, GetTime());
		bus_setis_stopping(bus, true);
		return;
	}
	float delta = GetFrameTime();
	float speed = bus_getspeed(bus);
	float move = speed*delta;
	float ratio = move/dist;
	dx = dx*ratio;
	dy = dy*ratio;
	bus_setposx(bus, xd+dx);
	bus_setposy(bus, yd+dy);
	*incx = (int)dx;
	*incy = (int)dy;
}
