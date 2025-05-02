/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "api.h"
#include "bus.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"

#define PADDING	200

Color random_color(void)
{
	Color c;
	int max, min;
	do
	{
		c = (Color){
			GetRandomValue(0, 127),  // Rouge
			GetRandomValue(0, 127),  // Vert
			GetRandomValue(0, 127),  // Bleu
			255                      // Opacité
		};
		max = (c.r > c.g) ? ((c.r > c.b) ? c.r : c.b) : ((c.g > c.b) ? c.g : c.b);
		min = (c.r < c.g) ? ((c.r < c.b) ? c.r : c.b) : ((c.g < c.b) ? c.g : c.b);
	} while ((max-min) < 30);
	return c;
}

int count_segments(const BusLine l)
{
	int c=0;
	List head = l;
	while (!list_is_empty(head))
	{
		if (gettype(list_getnode(head)) == ROUTE) ++c;
		head = list_getnext_node(head);
	}
	return c;
}

void collect_station_positions(const BusLine l, Vector2* points)
{
	int idx = 0;
	List head = l;
	while (!list_is_empty(head))
	{
		BusEntity* e = list_getnode(head);
		if (gettype(e) == STATION)
		{
			BusStation* s = e->bs;
			points[idx++] = (Vector2){ bs_getposx(s)+PADDING/2, bs_getposy(s)+PADDING };
		}
		head = list_getnext_node(head);
	}
}

void draw_bl(BusLine l, Font font, Color color)
{
	if (!l) return;
	int count = count_segments(l);
	#define SIZE	128
	if (count+1 > SIZE)
	{
		fprintf(stderr, "Nombre maximal de stations atteint\n");
		return;
	}
	Vector2 points[SIZE];
	collect_station_positions(l, points);
	// Dessiner les routes
	for (int i=0; i<count; ++i)
		DrawLineEx(points[i], points[i+1], 8.0f, WHITE);
	for (int i=0; i<count; ++i)
		DrawLineEx(points[i], points[i+1], 4.0f, color);
	#undef SIZE
	// Desinner les stations
	List head = l;
	while (!list_is_empty(head))
	{
		BusEntity* e = list_getnode(head);
		if (gettype(e) == INVALID)
		{
			fprintf(stderr, "Station invalide\n");
			head = list_getnext_node(head);
			continue;
		}
		BusStation* s = e->bs;
		int dx = bs_getposx(s) + PADDING/2;
		int dy = bs_getposy(s) + PADDING;
		DrawCircle(dx, dy, 8+4,	BLACK);
		DrawCircle(dx, dy, 8+2,	WHITE);
		DrawCircle(dx, dy, 8,	color);
		const char* name = bs_getname(s);
		if (!name) fprintf(stderr, "Station sans nom\n");
		Vector2 labelSize = MeasureTextEx(font, name, 16, 0);
		Vector2 labelPos = { dx-labelSize.x/2, dy-labelSize.y/2-24 };
		DrawTextEx(font, bs_getname(s), labelPos, 16, 0, BLACK);
		head = list_getnext_node(head);
	}
}

void draw_bus(Bus* bus, Color color)
{
	if (!bus) return;
	int dx = bus_getposx(bus)+PADDING/2;
	int dy = bus_getposy(bus)+PADDING;
	if (bus_getis_stopping(bus))
	{
		if ((GetTime()*4.0f - (int)(GetTime()*4.0f)) < 0.5f)
		{
			DrawCircle(dx, dy, 20, Fade(DARKBLUE, 0.4f));  // effet de halo clignotant
		}
	}
	DrawCircle(dx+3, dy+3, 16-4, Fade(BLACK, 0.2f));
	DrawCircle(dx, dy, 16, BLACK);
	DrawCircle(dx, dy, 16-2, WHITE);
	DrawCircle(dx, dy, 16-4, color);
	Font font = GetFontDefault();
	char label[8];
	snprintf(label, sizeof(label), "%d", bus_getid(bus));
	Vector2 labelSize = MeasureTextEx(font, label, 16, 0);
	Vector2 labelPos = { dx-labelSize.x/2, dy-labelSize.y/2 };
	DrawTextEx(font, TextFormat("%d", bus_getid(bus)), labelPos, 16, 0, WHITE);
}
