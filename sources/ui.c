/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#include <stdlib.h>
#include "api.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"

void draw_bl(List l, Font font, Color color)
{
	int padding = 200;
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
			points[idx++] = (Vector2){ bs_getposx(s)+padding, bs_getposy(s)+padding };
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
			int dx = bs_getposx(s)+padding;
			int dy = bs_getposy(s)+padding;
			DrawCircle(dx, dy, 8+4,	BLACK);
			DrawCircle(dx, dy, 8+2,	WHITE);
			DrawCircle(dx, dy, 8,	color);
			Vector2 labelPos = { dx-8-4, dy-16-8-4 };
			DrawTextEx(font, bs_getname(s), labelPos, 16, 0, BLACK);
		}
		head = _get_next_node(head);
	}
}
