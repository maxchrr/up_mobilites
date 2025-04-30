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
	if (gettype(_get_node(l)) == STATION)
		return _get_next_node(l); // Le suivant est une route
	else
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return l;
	}
}

List bl_getprev_br(List l)
{
	if (is_empty(l)) return NULL;
	if (gettype(_get_node(l)) == STATION)
		return _get_prev_node(l); // Le précédent est une route
	else
	{
		fprintf(stdout, "Attention déjà sur une route\n");
		return l;
	}
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

void bus_setdirection(BusPtr bus, BusDirection direction)
{
	bus->direction = direction;
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
	int padError = 2;
	List current;
	static float stopTime = 0;
	static bool isStopping = false;
	if (isStopping)
	{
		if (GetTime()-stopTime >= 2.0f)
			isStopping = false;
		return;
	}
	if (direction == DEP_TO_ARR)
	{
		current = bl_getnext_bs(bus_getbl(bus));
	}
	else if (direction == ARR_TO_DEP)
	{
		current = bl_getprev_bs(bus_getbl(bus));
	}
	*incx = 0; *incy = 0;
	if (!is_empty(current))
	{
		int xd = bus_getposx(bus);
		int yd = bus_getposy(bus);
		int xa = bl_getposx(current);
		int ya = bl_getposy(current);
		if (abs(xd-xa)<=padError && abs(yd-ya)<=padError)
		{
			bus_setbl(bus, current);
			print_bus(bus);
			stopTime = GetTime();
			isStopping = true;
		}
		else
		{
			float ratio = (xa != xd) ? abs((ya-yd)/(xa-xd)) : 0.0f;
			if (xa>xd)	*incx = padError;
			else if (xa<xd)	*incx = -padError;
			if (ya>yd)	*incy = padError*ratio;
			else if (yd<ya)	*incy = -padError*ratio;
			bus_setposx(bus, bus_getposx(bus)+*incx);
			bus_setposy(bus, bus_getposy(bus)+*incy);
		}
	}
}
