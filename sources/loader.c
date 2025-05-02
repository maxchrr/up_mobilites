/*
 * File loader
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "utils.h"
#include "loader.h"

BusLine init_from_file(int id, const char* path)
{
	FILE* f = fopen(path, "r");
	if (!f)
	{
		fprintf(stderr, "Erreur à la lecture du fichier %s\n", path);
		exit(1);
	}
	BusLine new_bl;
	init_list(&new_bl);
	BusStation* ps = NULL;
	BusStation* cs = NULL;
	char name[64];
	int x, y;
	int i = 1;
	while (fscanf(f, "%63[^,],%d,%d\n", name, &x, &y) == 3)
	{
		cs = create_bs(i++, name, x, y);
		if (ps != NULL) {
			BusRoute* r = create_br(id, ps, cs);
			new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
		}
		new_bl = insert_at_tail(new_bl, open_entity(STATION, cs));
		ps = cs;
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
	while ((entry = readdir(dir)) != NULL && count < MAX_TIMETABLES)
	{
		const char* name = entry->d_name;
		if (strncmp(name, "timetable", 9) == 0 && strstr(name, ".ini"))
		{
			char base[256];
			strncpy(base, name, sizeof(base) - 1);
			base[sizeof(base) - 1] = '\0';
			char* dot = strrchr(base, '.');
			if (dot) *dot = '\0';  // remove extension
			int line_id;
			if (sscanf(base+9, "%d", &line_id) != 1) continue;
			line_id = atoi(base+9);
			snprintf(buffer, sizeof(buffer), "%s/%s", path, entry->d_name);
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
