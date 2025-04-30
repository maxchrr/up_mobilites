/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "api.h"
#include "bus.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	800
#define WINDOW_TITLE	"UPmobilites"
#define MAX_TIMETABLES	32

typedef struct
{
	int id;
	List list;
} Timetable;

List init_from_file(int id, const char* path)
{
	FILE* f = fopen(path, "r");
	if (!f)
	{
		fprintf(stderr, "Erreur à la lecture du fichier %s\n", path);
		exit(1);
	}
	List new_bl;
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
			int line_id = atoi(base + 9);
			snprintf(buffer, sizeof(buffer), "%s/%s", path, entry->d_name);
			List bl = init_from_file(line_id, buffer);
			if (bl != NULL)
			{
				timetables[count].id = line_id;
				timetables[count].list = bl;
				count++;
			}
		}
	}
	closedir(dir);
	return count;
}

int main(void)
{
	Timetable timetables[MAX_TIMETABLES];
	int total = load_timetables(timetables, "vendor/timetables");
	if (total == 0)
	{
		fprintf(stderr, "Aucune ligne de bus chargée\n");
		return 1;
	}
	for (int i=0; i<total; ++i)
	{
		print_list(timetables[i].list);
	}

	BusPtr buses[MAX_TIMETABLES];
	int incx[MAX_TIMETABLES] = {0};
	int incy[MAX_TIMETABLES] = {0};

	for (int i=0; i<total; ++i)
	{
		buses[i] = init_bus(i+1, timetables[i].list);
		if (!buses[i])
		{
			fprintf(stderr, "Impossible d'initialiser un bus pour la ligne %d", timetables[i].id);
			return 1;
		}
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Font font = LoadFontEx("vendor/Luciole-Regular.ttf", 18, NULL, 255);
	if (font.texture.id == 0)
	{
		fprintf(stderr, "Police non chargée\n");
		return 1;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		for (int i=0; i<total; ++i)
		{
			draw_bl(timetables[i].list, font, DARKBLUE);
			draw_bus(buses[i], RED);
			bus_travel(buses[i], bus_getdirection(buses[i]), &incx[i], &incy[i]);
  		}

		EndDrawing();
	}

	UnloadFont(font);
	CloseWindow();
	for (int i=0; i<total; ++i) {
		destroy_bus(buses[i]);
		destroy_list(timetables[i].list);
	}
	return 0;
}
