/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
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

	Bus* buses[MAX_TIMETABLES];
	int incx[MAX_TIMETABLES] = {0};
	int incy[MAX_TIMETABLES] = {0};

	for (int i=0; i<total; ++i)
	{
		buses[i] = init_bus(i+1, timetables[i].list);
		if (!buses[i])
		{
			fprintf(stderr, "Impossible d'initialiser un bus pour la ligne %d\n", timetables[i].id);
			return 1;
		}
	}

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Font font = LoadFontEx("vendor/Luciole-Regular.ttf", 18, NULL, 255);
	if (font.texture.id == 0)
	{
		fprintf(stderr, "Police non chargée, utilisation de celle par défaut\n");
		font = GetFontDefault();
	}
	
	int* keys = calloc(total,sizeof(int));
	for (int i=0; i<total; ++i)
		keys[i] = KEY_ONE+i;  // Associe KEY_ONE, KEY_TWO, ..., en fonction de 'i'
	bool deleteMode = false;
	bool concatMode = false;
	bool loopMode = false;
	while (!WindowShouldClose())
	{
		// Mode suppression
		if (IsKeyPressed(KEY_D))
		{
			deleteMode = true;
			printf("[MODE] Suppression\n");
		}
		if (deleteMode)
		{
			int i = -1;
			for (int j=0; j<total; ++j)
			{
				if (IsKeyPressed(keys[j]))
				{
					deleteMode = false;
					i = j + 1;  // Assigner l'indice (1 pour KEY_ONE, 2 pour KEY_TWO, etc.)
					break;
				}
			}
			// Si c'est un indice valide et qu'il reste plus d'un chemin => supprime
			if (i > 0 && i <= total && length(timetables[i-1].list) > 3)
				bl_remove(timetables[i-1].list);
		}

		// Mode concaténation
		if (IsKeyPressed(KEY_C))
		{
			concatMode = true;
			printf("[MODE] Concaténation\n");
		}
		if (concatMode)
		{
			concatMode = false;
			timetables[0].list = bl_concat(timetables[0].list, timetables[1].list);;  // Réintégration de la liste concaténé
			timetables[1].list = NULL;  // La liste est oublié, ne pas libérer ici, mais à la fin
		}

		// Mode boucle
		if (IsKeyPressed(KEY_L))
		{
			loopMode = true;
			printf("[MODE] Boucle\n");
		}
		if (loopMode)
		{
			int i = -1;
			for (int j=0; j<total; ++j)
			{
				if (IsKeyPressed(keys[j]))
				{
					loopMode = false;
					i = j + 1;  // Assigner l'indice (1 pour KEY_ONE, 2 pour KEY_TWO, etc.)
					break;
				}
			}
			// Si c'est un indice valide => mettre en mode boucle
			if (i > 0 && i <= total)
				bus_loopback(buses[i], bus_getdirection(buses[i]));
		}

		// Boucle d'affichage
		float delta = GetFrameTime();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GRAY);

		if (deleteMode)
		{
			const char* text = "-- DELETE --";
			Vector2 textSize = MeasureTextEx(font, text, 20, 0);
			Vector2 textPos = { SCREEN_WIDTH-textSize.x-20, textSize.y };
			DrawTextEx(font, text, textPos, 20, 0, RED);
		}
		else if (concatMode)
		{
			const char* text = "-- CONCAT --";
			Vector2 textSize = MeasureTextEx(font, text, 20, 0);
			Vector2 textPos = { SCREEN_WIDTH-textSize.x-20, textSize.y };
			DrawTextEx(font, text, textPos, 20, 0, GREEN);
		}
		else if (loopMode)
		{
			const char* text = "-- LOOP --";
			Vector2 textSize = MeasureTextEx(font, text, 20, 0);
			Vector2 textPos = { SCREEN_WIDTH-textSize.x-20, textSize.y };
			DrawTextEx(font, text, textPos, 20, 0, BLUE);
		}

		for (int i=0; i<total; ++i)
		{
			draw_bl(timetables[i].list, font, timetables[i].color);
			if (buses[i]) draw_bus(buses[i], DARKPURPLE);
			bus_travel(buses[i], bus_getdirection(buses[i]), &incx[i], &incy[i], delta, GetTime());
		}

		EndDrawing();
	}

	free(keys);
	UnloadFont(font);
	CloseWindow();
	for (int i=0; i<total; ++i) {
		destroy_bus(buses[i]);
		destroy_timetable(&timetables[i]);
	}
	return 0;
}
