/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#include <stdlib.h>
#include "api.h"
#include "bus.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"

#define PADDING	200

void draw_bl(List l, Font font, Color color)
{
	int segCount = 0;
	Node* temp = l;
	while (!is_empty(_get_next_node(temp)))
	{
		BusEntity* e = _get_node(temp);
		if (e->route) ++segCount;
		temp = _get_next_node(temp);
	}
	int pCount = segCount+1;
	Vector2* points = malloc(sizeof(Vector2)*pCount);
	int idx = 0;
	Node* head = l;
	while (!is_empty(_get_next_node(head)))
	{
		BusEntity* e = _get_node(head);
		if (e->station)
		{
			BusStation* s = e->bs;
			points[idx++] = (Vector2){ bs_getposx(s)+PADDING, bs_getposy(s)+PADDING };
		}
		head = _get_next_node(head);
	}

	for (int i=0; i<idx-1; ++i)
		DrawLineEx(points[i], points[i+1], 8.0f, WHITE);
	for (int i=0; i<idx-1; ++i)
		DrawLineEx(points[i], points[i+1], 4.0f, color);

	free(points);

	head = l;
	while (!is_empty(_get_next_node(head)))
	{
		BusEntity* e = _get_node(head);
		if (e->station)
		{
			BusStation* s = e->bs;
			int dx = bs_getposx(s)+PADDING;
			int dy = bs_getposy(s)+PADDING;
			DrawCircle(dx, dy, 8+4,	BLACK);
			DrawCircle(dx, dy, 8+2,	WHITE);
			DrawCircle(dx, dy, 8,	color);
			Vector2 labelPos = { dx-8*2, dy-16-8*2 };
			DrawTextEx(font, bs_getname(s), labelPos, 16, 0, BLACK);
		}
		head = _get_next_node(head);
	}
}

void draw_bus(BusPtr bus, Color color)
{
	int dx = bus_getposx(bus)+PADDING;
	int dy = bus_getposy(bus)+PADDING;
	DrawCircle(dx, dy, 16, BLACK);
	DrawCircle(dx, dy, 16-2, WHITE);
	DrawCircle(dx, dy, 16-4, color);
}
