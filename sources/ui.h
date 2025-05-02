/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#pragma once

#include "bus.h"
#include "raylib.h"

Color  random_color(void);

int   count_segments(const BusLine l);
void  collect_station_positions(const BusLine l, Vector2* points);

void  draw_bl(BusLine l, Font font, Color color);
void  draw_bus(Bus* bus, Color color);
