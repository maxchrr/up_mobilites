/*
 * Lecteur de fichier
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#include "busline.h"
#include "list.h"

#define MAX_BUFFER_SIZE 256

typedef struct
{
	int* ids;           // Tableau d'identifiants
	char** files;       // Tableau de chemin de fichiers
	unsigned count;     // Nombre de fichiers traités
	unsigned capacity;  // Capacité des tableaux
} Timetables;

Timetables  load_dir(const char* path);
List        read_file(const int bl_id, const char* path);
void        free_timetables(Timetables t);
