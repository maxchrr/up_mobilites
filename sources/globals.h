/*
 * Variables globales
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#ifndef GLOBALS_H_
#define GLOBALS_H_

#define SCREEN_WIDTH		1200
#define SCREEN_HEIGHT		800
#define WINDOW_TITLE		"UPmobilites"

#define MAX_INPUT_LENGTH	16
#define MAX_NAME_LEN		32
#define MAX_COLORS		256
#define MAX_ATTEMPTS		1000  // limite de sécurité
#define PADDING			200  // pour l'interface
#define MAX_BUSES		100
#define MAX_BUFFER_SIZE		256

extern int freed_bus_ids[MAX_BUSES];
extern int freed_bus_count;
extern int bus_id;

#endif // GLOBALS_H_

