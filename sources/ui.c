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

void draw_bl(BusLine l, Font font, Color color)
{
	int segCount = 0;
	Node* temp = l;
	while (!is_empty(temp))
	{
		BusEntity* e = _get_node(temp);
		if (e->route) ++segCount;
		temp = _get_next_node(temp);
	}
	int pCount = segCount+1;
	Vector2 points[128];
	if (pCount > 128)
	{
		fprintf(stderr, "Trop de stations\n");
		return;
	}
	int idx = 0;
	temp = l;
	while (!is_empty(temp))
	{
		BusEntity* e = _get_node(temp);
		if (e->station)
		{
			BusStation* s = e->bs;
			points[idx++] = (Vector2){ bs_getposx(s)+PADDING/2, bs_getposy(s)+PADDING };
		}
		temp = _get_next_node(temp);
	}
	// Draw the lines connecting the stations
	for (int i=0; i<idx-1; ++i)
		DrawLineEx(points[i], points[i+1], 8.0f, WHITE);
	for (int i=0; i<idx-1; ++i)
		DrawLineEx(points[i], points[i+1], 4.0f, color);
	// Draw the stations
	temp = l;
	while (!is_empty(temp))
	{
		BusEntity* e = _get_node(temp);
		if (e->station)
		{
			BusStation* s = e->bs;
			int dx = bs_getposx(s)+PADDING/2;
			int dy = bs_getposy(s)+PADDING;
			DrawCircle(dx, dy, 8+4,	BLACK);
			DrawCircle(dx, dy, 8+2,	WHITE);
			DrawCircle(dx, dy, 8,	color);
			Vector2 labelSize = MeasureTextEx(font, bs_getname(s), 16, 0);
			Vector2 labelPos = { dx - labelSize.x / 2, dy - labelSize.y / 2 - 24 };
			DrawTextEx(font, bs_getname(s), labelPos, 16, 0, BLACK);
		}
		temp = _get_next_node(temp);
	}
}

void draw_bus(Bus* bus, Color color)
{
	if (!bus) return;
	int dx = bus_getposx(bus)+PADDING/2;
	int dy = bus_getposy(bus)+PADDING;
	DrawCircle(dx+3, dy+3, 16-4, Fade(BLACK, 0.2f));
	DrawCircle(dx, dy, 16, BLACK);
	DrawCircle(dx, dy, 16-2, WHITE);
	DrawCircle(dx, dy, 16-4, color);
}
