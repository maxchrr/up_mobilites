/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include "api.h"
#include "bus.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"
#include "loader.h"

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	800
#define WINDOW_TITLE	"UPmobilites"

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
			draw_bl(timetables[i].list, font, timetables[i].color);
			draw_bus(buses[i], DARKPURPLE);
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
