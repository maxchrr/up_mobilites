/*
 * Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#include "api.h"
#include "busline.h"

typedef enum { FORWARD, BACKWARD } BusDirection;
typedef struct Bus
{
	int id;                    // Identifiant du bus
	int posx, posy;            // Coordonées sur le plan
	int bl_id;                 // Identifant de la ligne de bus courante
	BusLine bl;                // Pointeur sur l'entité courante de la ligne de bus (une Station ou une Route)
	BusDirection direction;    // Sens de circulation du bus
	float speed;               // Vitesse de circulation
	float stop_time;           // Temps d'arrêt à une station
	unsigned is_stopping : 1;  // État de l'attente
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
unsigned      bus_getis_stopping(const Bus* bus);

/* Mutateur */
void  bus_setposx(Bus* bus, int value);
void  bus_setposy(Bus* bus, int value);
void  bus_setbl_id(Bus* bus, int value);
void  bus_setbl(Bus* bus, BusLine bl);
void  bus_setdirection(Bus* bus, BusDirection value);
void  bus_setspeed(Bus* bus, float value);
void  bus_setstop_time(Bus* bus, float value);
void  bus_setis_stopping(Bus* bus, unsigned value);

/* Méthodes */
void  bus_departure(Bus* bus, BusLine bl, BusDirection direction);
void  bus_travel(Bus* bus, BusDirection direction, int* incx, int* incy, float delta, double time);
