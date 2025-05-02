/*
 * Fonctions d'interface
 * Copyright (c) 2025 Max Charrier. All Rights Reserved.
 */
#ifndef UI_H_
#define UI_H_

#include "bus.h"
#include "raylib.h"

void draw_bl(BusLine l, Font font, Color color);

void draw_bus(Bus* bus, Color color);

#endif // UI_H_
