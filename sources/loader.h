/*
 * File loader
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef LOADER_H_
#define LOADER_H_

#include "bus.h"

#define MAX_TIMETABLES	32

typedef struct
{
	int id;
	BusLine list;
	Color color;
} Timetable;

BusLine init_from_file(int id, const char* path);

int   load_timetables(Timetable timetables[], const char* path);
void  destroy_timetable(Timetable* t);

#endif // LOADER_H_
