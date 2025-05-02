/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#ifndef UI_H_
#define UI_H_

#include "bus.h"
#include "raylib.h"

int   _count_segments(BusLine l);
void  _bs_getpos(BusLine l, Vector2* points);

void  draw_bl(BusLine l, Font font, Color color);
void  draw_bus(Bus* bus, Color color);

#endif // UI_H_
