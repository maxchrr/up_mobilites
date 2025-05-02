/*
 * File loader
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "api.h"
#include "utils.h"
#include "ui.h"
#include "loader.h"

BusLine init_from_file(int id, const char* path)
{
	FILE* f = fopen(path, "r");
	if (!f)
	{
		fprintf(stderr, "Erreur à la lecture du fichier %s\n", path);
		exit(1);
	}
	// Initialisation
	BusLine new_bl;
	init_list(&new_bl);
	BusStation* ps = NULL;
	BusStation* cs = NULL;
	BusRoute* r = NULL;
	char name[MAX_NAME_LEN];
	int x, y;
	int i = 1;
	char buffer[128];
	int cs_count = 0;
	// Lecture ligne par ligne
	while (fgets(buffer, sizeof(buffer), f))
	{
		if (buffer[0] == '\n' || buffer[0] == '#' || buffer[0] == '\r') continue;  // Ignorer les lignes vides ou commentaires
		if (sscanf(buffer, "%31[^,],%d,%d\n", name, &x, &y) == 3)
		{
			cs = create_bs(++i, name, x, y);
			++cs_count;
			if (!cs)
			{
				fprintf(stderr, "Erreur allocation BusStation\n");
				fclose(f);
				destroy_list(new_bl);
				return NULL;
			}
			if (ps != NULL) {
				r = create_br(id, ps, cs);
				if (!r)
				{
					fprintf(stderr, "Erreur allocation BusRoute\n");
					fclose(f);
					destroy_list(new_bl);
					return NULL;
				}
				new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
			}
			new_bl = insert_at_tail(new_bl, open_entity(STATION, cs));
			ps = cs;
		}
	}
	if (cs_count < 2) {
		fprintf(stderr, "Fichier %s invalide : moins de deux stations\n", path);
		destroy_list(new_bl);
		fclose(f);
		return NULL;
	}
	fclose(f);
	return new_bl;
}

int load_timetables(Timetable timetables[], const char* path)
{
	DIR* dir = opendir(path);
	if (!dir)
	{
		perror("Erreur ouverture dossier");
		return 0;
	}
	struct dirent* entry;
	int count = 0;
	char buffer[256];
	// Lecture du contenu du dossier
	while ((entry = readdir(dir)) != NULL && count < MAX_TIMETABLES)
	{
		const char* name = entry->d_name;
		if (name[0] == '.' || strstr(name, "~") || strstr(name, ".swp") || strstr(name, ".bak")) continue;
		// Lecture du nom du fichier
		if (strncmp(name, "timetable", 9) == 0 && strstr(name, ".ini") == name + strlen(name)-4)
		{
			char base[256];
			strncpy(base, name, sizeof(base) - 1);
			base[sizeof(base) - 1] = '\0';
			// Supprimer l'extension
			char* dot = strrchr(base, '.');
			if (dot) *dot = '\0';
			// Lire numéro de ligne
			int line_id;
			if (sscanf(base+9, "%d", &line_id) != 1) continue;
			line_id = atoi(base+9);  // conversion
			snprintf(buffer, sizeof(buffer), "%s/%s", path, entry->d_name);
			// Conversion en BusLine
			BusLine bl = init_from_file(line_id, buffer);
			if (bl != NULL)
			{
				timetables[count].id = line_id;
				timetables[count].list = bl;
				timetables[count].color = random_color();
				++count;
			}
		}
	}
	closedir(dir);
	return count;
}

void destroy_timetable(Timetable* t)
{
	if (!t) return;
	destroy_list(t->list);
	t->list = NULL;
	t->id = 0;
	t->color = (Color){0};  // Pour nettoyage visuel
}
