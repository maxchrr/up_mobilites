/*
 * Module de gestion de bus
 *
 * Copyright 2025 Max Charrier, Emilio Decaix-Massiani. Licensed under the Educational
 * Community License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/ECL-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#pragma once

#include "list.h"

typedef enum { FORWARD, BACKWARD } BusDirection;
typedef struct Bus
{
	int id;                    // Identifiant du bus
	int posx, posy;            // Coordonées sur le plan
	float fx, fy;              // Position réelle du bus
	int bl_id;                 // Identifant de la ligne de bus courante
	List bl;                // Pointeur sur l'entité courante de la ligne de bus (une Station ou une Route)
	BusDirection direction;    // Sens de circulation du bus
	float speed;               // Vitesse de circulation
	float stop_time;           // Temps d'arrêt à une station
	int looping_count;         // Nombre de tours bouclé
	unsigned is_stopping : 1;  // État de l'attente
} Bus;

Bus*  init_bus(int id, List bl);
void  print_bus(const Bus* bus);
void  destroy_bus(Bus* bus);

/* Accesseur */
int           bus_getid(const Bus* bus);
int           bus_getposx(const Bus* bus);
int           bus_getposy(const Bus* bus);
float         bus_getfx(const Bus* bus);
float         bus_getfy(const Bus* bus);
int           bus_getbl_id(const Bus* bus);
List          bus_getbl(const Bus* bus);
BusDirection  bus_getdirection(const Bus* bus);
float         bus_getspeed(const Bus* bus);
float         bus_getstop_time(const Bus* bus);
int           bus_getlooping_count(const Bus* bus);
unsigned      bus_getis_stopping(const Bus* bus);

/* Mutateur */
void  bus_setposx(Bus* bus, int value);
void  bus_setposy(Bus* bus, int value);
void  bus_setfx(Bus* bus, float value);
void  bus_setfy(Bus* bus, float value);
void  bus_setbl_id(Bus* bus, int value);
void  bus_setbl(Bus* bus, List bl);
void  bus_setdirection(Bus* bus, BusDirection value);
void  bus_setspeed(Bus* bus, float value);
void  bus_setstop_time(Bus* bus, float value);
void  bus_setlooping_count(Bus* bus, int value);
void  bus_setis_stopping(Bus* bus, unsigned value);

/* Méthodes */
void  bus_departure(Bus* bus, List bl, BusDirection direction);
void  bus_travel(Bus* bus, BusDirection direction, int* incx, int* incy, float delta, double time);
void  bus_loopback(Bus* bus);
