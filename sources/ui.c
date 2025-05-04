/*
 * Module d'affichage d'interface et de gestion d'entrée clavier
 *
 * Copyright 2025 Max Charrier, Emilio Decaix-Massiani. Licensed under the Educational
 * Community License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License at
 *
 * http://opensource.org/licenses/ECL-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "globals.h"
#include "api.h"
#include "list.h"
#include "bus.h"
#include "raylib.h"
#include "ui.h"

static Color used_colors[MAX_COLORS];
static int color_count = 0;
int freed_bus_ids[MAX_BUSES];
int freed_bus_count = 0;
int bus_id = 0;

int find_line_index(BusLine* lines, unsigned count, int id)
{
	for (unsigned i=0; i<count; ++i)
	{
		if (lines[i].id == id)
			return (int)i;
	}
	return -1;  // introuvable
}

void handle_command(const char* cmd, BusLine* lines, unsigned line_count)
{
	if (!cmd || strlen(cmd) < 2) return;
	char mode = cmd[1];
	int id = atoi(&cmd[2]);
	int i = find_line_index(lines, line_count, id);
	bool is_line_exist = (i != -1) && !list_is_empty(lines[i].list);
	if (!is_line_exist) return;
	// Mode insertion de bus
	if (mode == 'i')
	{
		int new_id = (freed_bus_count > 0)
			? freed_bus_ids[--freed_bus_count]
			: ++bus_id;
		Bus* new_bus = init_bus(new_id, lines[i].list);
		if (new_bus)
			bl_add_bus(&lines[i], new_bus);
	}
	// Mode suppression de bus
	else if (mode == 'd')
	{
		char* slash_pos = strchr(cmd, '/');
		if (slash_pos)
		{
			int bus_id = atoi(slash_pos+1);
			bl_remove_bus(&lines[i], bus_id);
		}
	}
	// Mode concaténation de ligne de bus
	else if (mode == 'c')
	{
		char* slash_pos = strchr(cmd, '/');
		if (slash_pos)
		{
			int id2 = atoi(slash_pos+1);
			int j = find_line_index(lines, line_count, id2);
			bool valid = (j != -1) && (id != id2);
			bool non_empty2 = !list_is_empty(lines[j].list);
			if (valid && non_empty2)
				bl_concat(&lines[i], &lines[j]);
		}
	}
	// Mode suppresion de chemin de ligne de bus
	else if (mode  == 'r')
	{
		bool is_removable = length(lines[i].list) > 3;
		if (is_removable)
			bl_remove_tail(lines[i].list);
	}
}

float color_distance(Color a, Color b)
{
	int dr = a.r - b.r;
	int dg = a.g - b.g;
	int db = a.b - b.b;
	return sqrtf(dr * dr + dg * dg + db * db);  // distance euclidienne
}

Color random_color(void)
{
	Color c;
	int max, min;
	#define MIN_DIST	100.0f  // distance minimum acceptable
	int attempts = 0;
	int found = 0;
	while (attempts < MAX_ATTEMPTS && !found)
	{
		c = (Color){
			GetRandomValue(0, 127),  // Rouge
			GetRandomValue(0, 127),  // Vert
			GetRandomValue(0, 127),  // Bleu
			255                      // Opacité
		};
		max = (c.r > c.g) ? ((c.r > c.b) ? c.r : c.b) : ((c.g > c.b) ? c.g : c.b);
		min = (c.r < c.g) ? ((c.r < c.b) ? c.r : c.b) : ((c.g < c.b) ? c.g : c.b);
		if ((max-min) >= 30)
		{ 
			int i = 0;
			unsigned too_close = 0;
			while (i < color_count && !too_close)
			{
				float dist = color_distance(c, used_colors[i]);
				if (dist < MIN_DIST) too_close = 1;
				++i;
			};
			if (!too_close) found = 1;
		}
		++attempts;
	}
	#undef MIN_DIST
	if (color_count < MAX_COLORS && found) used_colors[color_count++] = c;
	return c;
}

int count_segments(const List l)
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

void collect_station_positions(const List l, Vector2* points)
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

void draw_bl(BusLine bl, Font font, Color color)
{
	if (!bl.list) return;
	int count = count_segments(bl.list);
	#define SIZE	128
	if (count+1 > SIZE)
	{
		fprintf(stderr, "Nombre maximal de stations atteint\n");
		return;
	}
	Vector2 points[SIZE];
	collect_station_positions(bl.list, points);
	// Afficher le nom de la ligne à gauche de la première station
	if (count >= 0 && bl.id)
	{
		char label[MAX_NAME_LEN];
		snprintf(label, MAX_NAME_LEN, "Ligne %d", bl.id);
		Vector2 nameSize = MeasureTextEx(font, label, 12, 0);
		Vector2 namePos =
		{
			points[0].x - nameSize.x - 16,  // gauche de la première station
			points[0].y - nameSize.y / 2  // centre
		};
		Rectangle box = {
			namePos.x - 8,
			namePos.y - 2,
			nameSize.x + 8,
			nameSize.y + 4
		};
		DrawRectangleRounded(box, 0.2f, 8, Fade(color, 0.2f));
		DrawRectangleRoundedLines(box, 0.2f, 8, color);
		Vector2 labelPos = {
			box.x + (box.width - nameSize.x) / 2,  // centre
			box.y + (box.height - nameSize.y) / 2   // centre
		};
		DrawTextEx(font, label, labelPos, 12, 0, BLACK);
	}

	// Dessiner les routes
	for (int i=0; i<count; ++i)
		DrawLineEx(points[i], points[i+1], 8.0f, WHITE);
	for (int i=0; i<count; ++i)
		DrawLineEx(points[i], points[i+1], 4.0f, color);
	#undef SIZE
	// Desinner les stations
	List head = bl.list;
	while (!list_is_empty(head))
	{
		BusEntity* e = list_getnode(head);
		if (gettype(e) != INVALID)
		{
			BusStation* s = e->bs;
			int dx = bs_getposx(s) + PADDING/2;
			int dy = bs_getposy(s) + PADDING;
			DrawCircle(dx, dy, 8+4,	BLACK);
			DrawCircle(dx, dy, 8+2,	WHITE);
			DrawCircle(dx, dy, 8,	color);
			const char* name = bs_getname(s);
			if (!name) fprintf(stderr, "Station sans nom\n");
			Vector2 labelSize = MeasureTextEx(font, name, 16, 0);
			Vector2 labelPos =
			{
				dx - labelSize.x / 2,  // centre
				dy - labelSize.y / 2 - 24  // centre légèrement au dessus
			};
			DrawTextEx(font, bs_getname(s), labelPos, 16, 0, BLACK);
		}
		else
			fprintf(stderr, "Station invalide\n");
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
		float blink = fmodf(GetTime(), 4.0f);
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
	Vector2 labelPos =
	{
		dx - labelSize.x / 2,  // centre du cercle
		dy - labelSize.y / 2  // centre du cercle
	};
	DrawTextEx(font, label, labelPos, 16, 0, WHITE);
}
