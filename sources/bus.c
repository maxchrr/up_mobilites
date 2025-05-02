/*
 * Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "api.h"
#include "bus.h"

Bus* init_bus(int id, BusLine bl)
{
	Bus* new_bus = malloc(sizeof(Bus));
	if (!new_bus) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	new_bus->id = id;
	new_bus->speed = 100.0f;
	new_bus->stop_time = 0.0f;
	new_bus->is_stopping = false;
	// Position initiale
	int x = bl_getcurrent_posx(bl);
	int y = bl_getcurrent_posy(bl);
	new_bus->fx = x;
	new_bus->fy = y;
	new_bus->posx = x;
	new_bus->posy = y;
	new_bus->looping_count = 0;
	bus_departure(new_bus, bl, FORWARD);
	return new_bus;
}

void print_bus(const Bus* bus)
{
	fprintf(
		stdout,
		"[BUS #%d] ROUTE #%d \"%s\" (%d,%d) --> %s (%d)\n",
		bus_getid(bus),
		bus_getbl_id(bus),
		bs_getname(list_getnode(bus->bl)->bs),
		bus_getposx(bus),
		bus_getposy(bus),
		(bus_getdirection(bus) == FORWARD) ? "Avance" : "Recule",
		bus_getlooping_count(bus)
	);
}

void destroy_bus(Bus* bus)
{
	// ATTENTION : ne pas libérer la ligne si elle est partagée entre plusieurs bus => voir destroy_timetable();
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

float bus_getfx(const Bus* bus)
{
	return bus->fx;
}

float bus_getfy(const Bus* bus)
{
	return bus->fy;
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

int bus_getlooping_count(const Bus* bus)
{
	return bus->looping_count;
}

unsigned bus_getis_stopping(const Bus* bus)
{
	return bus->is_stopping;
}

void bus_setposx(Bus* bus, int value)
{
	bus->posx = value;
}

void bus_setposy(Bus* bus, int value)
{
	bus->posy = value;
}

void bus_setfx(Bus* bus, float value)
{
	bus->fx = value;
}

void bus_setfy(Bus* bus, float value)
{
	bus->fy = value;
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

void bus_setlooping_count(Bus* bus, int value)
{
	bus->looping_count = value;
}

void bus_setis_stopping(Bus* bus, unsigned value)
{
	bus->is_stopping = value;
}

void bus_departure(Bus* bus, BusLine bl, BusDirection direction)
{
	Node* next_node = list_getnext_node(bl);
	BusEntity* next = next_node ? list_getnode(next_node) : NULL;
	bus_setbl_id(bus, (next && gettype(next) == ROUTE) ? br_getbl_id(next->br) : -1);
	bus_setbl(bus, bl);
	bus_setdirection(bus, direction);
	int x = bl_getcurrent_posx(bl);
	int y = bl_getcurrent_posy(bl);
	bus_setfx(bus, x);
	bus_setfy(bus, y);
	bus_setposx(bus, x);
	bus_setposy(bus, y);
	print_bus(bus);
}

void bus_travel(Bus* bus, BusDirection direction, int* incx, int* incy, float delta, double time)
{
	if (!bus) return;
	if (bus_getis_stopping(bus))
	{
		if (time-bus_getstop_time(bus) >= 2.0f)
			bus_setis_stopping(bus, 0);
		return;
	}
	BusLine current = (direction == FORWARD)
		? bl_getnext_bs(bus_getbl(bus))
		: bl_getprev_bs(bus_getbl(bus));
	if (list_is_empty(current))
	{
		bus_loopback(bus);
		bus_setlooping_count(bus, bus_getlooping_count(bus)+1);
		return;
	}
	int xa = bl_getcurrent_posx(current);
	int ya = bl_getcurrent_posy(current);
	float dx = xa - bus_getfx(bus);
	float dy = ya - bus_getfy(bus);
	float dist = sqrtf(dx * dx + dy * dy);
	// Arrêt si proche
	if (dist < 1.0f)
	{
		print_bus(bus);
		bus_setbl(bus, current);
		bus_setstop_time(bus, time);
		bus_setis_stopping(bus, 1);
		return;
	}
	float move = bus_getspeed(bus) * delta;
	// Normalisation
	dx = (dx/dist) * move;
	dy = (dy/dist) * move;
	bus_setfx(bus, bus_getfx(bus) + dx);
	bus_setfy(bus, bus_getfy(bus) + dy);
	// Déplacement appliquer à la frame courante
	int newx = (int)(bus_getfx(bus) + 0.5f);
	int newy = (int)(bus_getfy(bus) + 0.5f);
	*incx = newx - bus_getposx(bus);
	*incy = newy - bus_getposy(bus);
	// Appliquer le déplacement
	bus_setposx(bus, newx);
	bus_setposy(bus, newy);
}

void bus_loopback(Bus* bus)
{
	if (!bus) return;
	bus_setdirection(bus, (bus_getdirection(bus) == FORWARD) ? BACKWARD : FORWARD); // Changement de direction automatique au terminus
}
