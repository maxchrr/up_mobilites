/*
 * Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef BUS_H_
#define BUS_H_

#include <stdbool.h>
#include "api.h"
#include "list.h"

typedef enum { DEP_TO_ARR, ARR_TO_DEP} BusDirection;
typedef struct Bus
{
	int id;                  // Identifiant du bus
	int posx, posy;          // Coordonées sur le plan
	int bl_id;               // Identifant de la ligne de bus courante
	List bl;                 // Pointeur sur l'entité courante de la ligne de bus (une Station ou une Route)
	BusDirection direction;  // Sens de circulation du bus
	float speed;             // Vitesse de circulation
	float stop_time;         // Temps d'arrêt à une station
	bool is_stopping;        // État de l'attente
} Bus;
typedef struct Bus* BusPtr;

BusPtr  init_bus(int id, List bl);
void    print_bus(const BusPtr bus);
void    destroy_bus(BusPtr bus);

/* Accesseur */
int           bus_getid(const BusPtr bus);
int           bus_getposx(const BusPtr bus);
int           bus_getposy(const BusPtr bus);
int           bus_getbl_id(const BusPtr bus);
List          bus_getbl(const BusPtr bus);
BusDirection  bus_getdirection(const BusPtr bus);
float         bus_getspeed(const BusPtr bus);
float         bus_getstop_time(const BusPtr bus);
bool          bus_getis_stopping(const BusPtr bus);

int   bl_getposx(List l);
int   bl_getposy(List l);
List  bl_getnext_bs(List l);
List  bl_getprev_bs(List l);
List  bl_getnext_br(List l);
List  bl_getprev_br(List l);

/* Mutateur */
void  bus_setposx(BusPtr bus, int value);
void  bus_setposy(BusPtr bus, int value);
void  bus_setbl_id(BusPtr bus, int value);
void  bus_setbl(BusPtr bus, List bl);
void  bus_setdirection(BusPtr bus, BusDirection value);
void  bus_setspeed(BusPtr bus, float value);
void  bus_setstop_time(BusPtr bus, float value);
void  bus_setis_stopping(BusPtr bus, bool value);

/* Méthodes */
void  bus_departure(BusPtr bus, List bl, BusDirection direction);
void  bus_travel(BusPtr bus, BusDirection direction, int* incx, int* incy);

#endif // BUS_H_
