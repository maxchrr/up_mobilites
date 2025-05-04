/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"
#include "loader.h"
#include "ui.h"
#include "raylib.h"

int main(void)
{
	// Allocation et lecture
	Timetables timetables = load_dir("vendor/timetables");
	if (timetables.count == 0)
	{
		fprintf(stderr, "Aucun fichier de ligne de bus trouvée\n");
		free_timetables(timetables);
		return EXIT_FAILURE;
	}
	printf("%d fichiers trouvées\n", timetables.count);
	BusLine* lines = calloc(timetables.count, sizeof(BusLine));
	int incx[100] = {0};
	int incy[100] = {0};
	if (!lines)
	{
		fprintf(stderr, "Memory allocation failed\n");
		free_timetables(timetables);
		return EXIT_FAILURE;
	}
	for (unsigned i=0; i<timetables.count; ++i)
	{
		printf("Lecture de %s (%d)\n", timetables.files[i], timetables.ids[i]);
		lines[i] = create_bl(timetables.ids[i]);
		init_bl(&lines[i], timetables.files[i]);
		print_bl(lines[i]);
	}

	// Affichage
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Font font = LoadFontEx("vendor/Luciole-Regular.ttf", 18, NULL, 255);
	if (font.texture.id == 0)
	{
		fprintf(stderr, "Police non chargée, bascule sur celle par défaut\n");
		font = GetFontDefault();
	}

	// Commandes et modes
	char cmd[MAX_INPUT_LENGTH] = {0};
	int cmd_index = 0;
	bool command_mode = false;
	bool paused = false;

	while (!WindowShouldClose())
	{
		// Mode pause
		if (IsKeyPressed(KEY_SPACE))
		{
			paused = !paused;
		}
		// Mode commande
		int ch = GetCharPressed();
		while (ch > 0)
		{
			if (command_mode && cmd_index < MAX_INPUT_LENGTH - 1)
				cmd[cmd_index++] = (char)ch;
			else if (ch == ':')
			{
				command_mode = true;
				cmd_index = 0;
				cmd[cmd_index++] = ':';
			}
			ch = GetCharPressed();
		}
		if (command_mode && IsKeyPressed(KEY_BACKSPACE))
		{
			cmd[--cmd_index] = '\0';
		}
		else if (command_mode && IsKeyPressed(KEY_ENTER))
		{
			cmd[cmd_index] = '\0';
			handle_command(cmd, lines, timetables.count);
			command_mode = false;
			memset(cmd, 0, sizeof(cmd));
			cmd_index = 0;
		}

		// Variables temporelles
		float delta = GetFrameTime();
		double time = GetTime();
		int fps = GetFPS();

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText(TextFormat("FPS: %d", fps), 10, 10, 20, GRAY);

		if (paused)
		{
			const char* text = "-- PAUSE --";
			Vector2 textSize = MeasureTextEx(font, text, 20, 0);
			Vector2 textPos =
			{
				SCREEN_WIDTH - textSize.x - 20,  // à droite légèrement décalé
				textSize.y  // en haut
			};
			DrawTextEx(font, text, textPos, 20, 0, DARKBLUE);
		}
		else if (command_mode)
		{
			const char* text = cmd;
			Vector2 textPos =
			{
				6,  // à gauche, légérément décalé
				SCREEN_HEIGHT - 20  // en bas, un peu au dessus du bord
			};
			DrawTextEx(font, text, textPos, 14, 0, DARKGRAY);
		}

		for (unsigned i=0; i<timetables.count; ++i)
		{
			// Dessiner ligne de bus
			draw_bl(lines[i], font, lines[i].color);
			// Dessiner bus et déplacement bus
			for (unsigned j=0; j<lines[i].bus_count; ++j)
			{
				Bus* bus = lines[i].bus_arr[j];
				draw_bus(bus, DARKPURPLE, paused);
				if (!paused)
				{
					BusDirection direction = bus_getdirection(bus);
					bus_travel(bus, direction, &incx[i], &incy[i], delta, time);
				}
			}
		}

		EndDrawing();
	}

	// Libération mémoire
	UnloadFont(font);
	CloseWindow();
	for (unsigned i=0; i<timetables.count; ++i)
		destroy_bl(lines[i]);
	free(lines);
	free_timetables(timetables);
	return EXIT_SUCCESS;
}
