/*
 * Lecteur de dossier et fichiers
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
