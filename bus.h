/*
 * Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#include <stdbool.h>
#include "api.h"
#include "list.h"

typedef List BusLine;  // Pour plus de lisibilité

BusLine bus_line_create(int id, const char *name);
void bus_line_add_station(BusLine *line, BusStation *station);

typedef enum { DEP_TO_ARR, ARR_TO_DEP} BusDirection;
typedef struct Bus
{
	int id;                  // Identifiant du bus
	int posx, posy;          // Coordonées sur le plan
	int bl_id;               // Identifant de la ligne de bus courante
	BusLine bl;              // Pointeur sur l'entité courante de la ligne de bus (une Station ou une Route)
	BusDirection direction;  // Sens de circulation du bus
	float speed;             // Vitesse de circulation
	float stop_time;         // Temps d'arrêt à une station
	bool is_stopping;        // État de l'attente
} Bus;

Bus*  init_bus(int id, BusLine bl);
void  print_bus(const Bus* bus);
void  destroy_bus(Bus* bus);

/* Accesseur */
int           bus_getid(const Bus* bus);
int           bus_getposx(const Bus* bus);
int           bus_getposy(const Bus* bus);
int           bus_getbl_id(const Bus* bus);
BusLine       bus_getbl(const Bus* bus);
BusDirection  bus_getdirection(const Bus* bus);
float         bus_getspeed(const Bus* bus);
float         bus_getstop_time(const Bus* bus);
bool          bus_getis_stopping(const Bus* bus);

int      bl_getcurrent_posx(BusLine l);
int      bl_getcurrent_posy(BusLine l);
BusLine  bl_getnext_bs(BusLine l);
BusLine  bl_getprev_bs(BusLine l);
BusLine  bl_getnext_br(BusLine l);
BusLine  bl_getprev_br(BusLine l);

/* Mutateur */
void  bus_setposx(Bus* bus, int value);
void  bus_setposy(Bus* bus, int value);
void  bus_setbl_id(Bus* bus, int value);
void  bus_setbl(Bus* bus, BusLine bl);
void  bus_setdirection(Bus* bus, BusDirection value);
void  bus_setspeed(Bus* bus, float value);
void  bus_setstop_time(Bus* bus, float value);
void  bus_setis_stopping(Bus* bus, bool value);

/* Méthodes */
void  bus_departure(Bus* bus, BusLine bl, BusDirection direction);
void  bus_travel(Bus* bus, BusDirection direction, int* incx, int* incy, float delta, double time);

BusLine create_from_existente(BusLine line1, BusLine line2);
