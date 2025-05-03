/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "loader.h"
#include "ui.h"
#include "raylib.h"

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	800
#define WINDOW_TITLE	"UPmobilites"

#define MAX_INPUT_LENGTH 16

int bus_id = 0;

void handle_command(const char* command, BusLine* lines, unsigned line_count)
{
	// Mode insertion de bus
	if (command[0] == ':' && command[1] == 'i')
	{
		int line_num = atoi(&command[2]);
		bool is_in_range = line_num >= 1 && line_num <= (int)line_count;
		bool is_line_exist = !list_is_empty(lines[line_num-1].list);
		if (is_in_range && is_line_exist)
		{
			Bus* new_bus = init_bus(++bus_id, lines[line_num-1].list);
			if (new_bus)
				bl_add_bus(&lines[line_num-1], new_bus);
		}
	}
	// Mode suppression de bus
	else if (command[0] == ':' && command[1] == 'd')
	{
		int line_num = atoi(&command[2]);
		bool is_in_range = line_num >= 1 && line_num <= (int)line_count;
		bool is_line_exist = !list_is_empty(lines[line_num-1].list);
		if (is_in_range && is_line_exist)
		{
			bl_remove_bus(&lines[line_num-1]);
		}
	}
	// Mode concaténation de ligne de bus
	else if (command[0] == ':' && command[1] == 'c')
	{
		char* slash_pos = strchr(command, '/');
		if (slash_pos)
		{
			int line_num1 = atoi(&command[2]);
			int line_num2 = atoi(slash_pos+1);
			bool is_in_range1 = line_num1 >= 1 && line_num1 <= (int)line_count;
			bool is_in_range2 = line_num2 >= 1 && line_num2 <= (int)line_count;
			bool is_in_range = is_in_range1 && is_in_range2;
			bool is_line_exist1 = !list_is_empty(lines[line_num1-1].list);
			bool is_line_exist2 = !list_is_empty(lines[line_num2-1].list);
			bool is_line_exist = is_line_exist1 && is_line_exist2;
			bool is_different = line_num1 != line_num2;
			if (is_in_range && is_line_exist && is_different)
			{
				lines[line_num1-1].list = bl_concat(lines[line_num1-1].list, lines[line_num2-1].list);;  // Réintégration de la liste concaténé
				lines[line_num2-1].list = NULL;  // La liste est oublié, ne pas libérer ici, mais à la fin
			}
		}
	}
	// Mode suppresion de chemin de ligne de bus
	else if (command[0] == ':' && command[1] == 'r')
	{
		int line_num = atoi(&command[2]);
		bool is_in_range = line_num >= 1 && line_num <= (int)line_count;
		bool is_line_exist = !list_is_empty(lines[line_num-1].list);
		bool is_removable = length(lines[line_num-1].list) > 3;
		if (is_in_range && is_line_exist && is_removable)
		{
			bl_remove(lines[line_num-1].list);
		}
	}
}

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
	//bool deleteMode = false;

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
			Vector2 textPos = { SCREEN_WIDTH-textSize.x-20, textSize.y };
			DrawTextEx(font, text, textPos, 20, 0, DARKBLUE);
		}
		else if (command_mode)
		{
			const char* text = cmd;
			Vector2 textSize = MeasureTextEx(font, text, 20, 0);
			Vector2 textPos = { textSize.x, SCREEN_HEIGHT-textSize.y-10 };
			DrawTextEx(font, text, textPos, 20, 0, DARKGRAY);
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
