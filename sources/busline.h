/*
 * Ligne de Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#include "list.h"
#include "bus.h"
#include "raylib.h"

typedef struct
{
	int id;                 // Identifiant de la ligne
	List list;              // Liste de station et tronçon
	Color color;            // Couleur de la ligne (pour l'affichage)
	Bus** bus_arr;          // Tableau des bus actuellement sur la ligne
	unsigned bus_count;     // Nombre de bus sur la ligne
	unsigned arr_capacity;  // Capacité du tableau
} BusLine;

BusLine  create_bl(int id);
void     init_bl(BusLine* bl, const char* path);
void     print_bl(BusLine bl);
void     destroy_bl(BusLine bl);

int      bl_getcurrent_posx(List l);
int      bl_getcurrent_posy(List l);
List     bl_getnext_bs(List l);
List     bl_getprev_bs(List l);
List     bl_getnext_br(List l);
List     bl_getprev_br(List l);

void  bl_add_bus(BusLine* bl, Bus* bus);
void  bl_remove_bus(BusLine* bl, int bus_id);

List  bl_remove_tail(List l);
List  bl_concat(List l1, List l2);
