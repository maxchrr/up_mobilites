/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#pragma once

#include "globals.h"
#include "list.h"
#include "busline.h"
#include "bus.h"
#include "raylib.h"

void handle_command(const char* cmd, BusLine* lines, unsigned line_count);

Color  random_color(void);

int   count_segments(const List l);
void  collect_station_positions(const List l, Vector2* points);

void  draw_bl(BusLine bl, Font font, Color color);
void  draw_bus(Bus* bus, Color color, int paused);
