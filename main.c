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
 
void add_line_to_array(BusLine ***lines, int *count, BusLine *new_line) {
	BusLine **tmp = realloc(*lines, sizeof(BusLine*) * (*count + 1));
	if (tmp) {
		*lines = tmp;
		(*lines)[*count] = new_line;
		(*count)++;
	} else {
		fprintf(stderr, "Erreur mémoire lors de l'ajout de la ligne fusionnée\n");
	}
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
 
	
	if (total >= 2) {
		BusLine fusion = create_from_existente(timetables[0].list, timetables[1].list);
		if (fusion) {
			printf("Ligne fusionnée créée avec succès entre %d et %d\n",
				timetables[0].id, timetables[1].id);
			print_list(fusion);
		}
	}
 
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);
 
	Font font = LoadFont("vendor/Luciole-Regular.ttf");
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
 
		for (int i=0; i<total; ++i)
		{
			draw_bl(timetables[i].list, font, BLACK);
			draw_bus(buses[i], RED);
		}
 
		EndDrawing();
	}
 
	CloseWindow();
	return 0;
}