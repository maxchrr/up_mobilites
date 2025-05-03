/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "api.h"
#include "bus.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"

#define MAX_COLORS	256
#define MAX_ATTEMPTS	1000  // limite de sécurité
#define PADDING		200

static Color used_colors[MAX_COLORS];
static int color_count = 0;

float color_distance(Color a, Color b)
{
	int dr = a.r - b.r;
	int dg = a.g - b.g;
	int db = a.b - b.b;
	return sqrtf(dr * dr + dg * dg + db * db);  // distance euclidienne
}

Color random_color(void)
{
	SetRandomSeed((unsigned int)time(NULL));
	Color c;
	int max, min;
	#define MIN_DIST	100.0f  // minimum acceptable distinct
	int attempts = 0;
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
		if ((max-min) < 30) continue;
		int too_close = 0;
		for (int i=0; i<color_count; ++i)
		{
			if (color_distance(c, used_colors[i]) < MIN_DIST)
			{
				too_close = 1;
				break;
			}
		}
		if (!too_close) break;
	} while (++attempts < MAX_ATTEMPTS);
	#undef MIN_DIST
	if (color_count < MAX_COLORS) used_colors[color_count++] = c;
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

void draw_bus(Bus* bus, Color color, int paused)
{
	if (!bus) return;
	int dx = bus_getposx(bus)+PADDING/2;
	int dy = bus_getposy(bus)+PADDING;
	if (bus_getis_stopping(bus) && !paused)
	{
		float blink = GetTime() * 4.0f;
		if ((blink - (int)blink) < 0.5f) {
			DrawCircle(dx, dy, 20, Fade(DARKBLUE, 0.4f));  // halo clignotant
		}
	}
	// Ombre
	DrawCircle(dx+3, dy+3, 16-4, Fade(BLACK, 0.2f));
	// Contour et remplissage
	DrawCircle(dx, dy, 16, BLACK);
	DrawCircle(dx, dy, 16-2, WHITE);
	DrawCircle(dx, dy, 16-4, color);
	// Texte
	char label[8];
	snprintf(label, sizeof(label), "%d", bus_getid(bus));
	Font font = GetFontDefault();
	Vector2 labelSize = MeasureTextEx(font, label, 16, 0);
	Vector2 labelPos = { dx-labelSize.x/2, dy-labelSize.y/2 };
	DrawTextEx(font, TextFormat("%d", bus_getid(bus)), labelPos, 16, 0, WHITE);
}
