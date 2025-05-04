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
#pragma once

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
