/*
 * File loader
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef LOADER_H_
#define LOADER_H_

#include "list.h"

#define MAX_TIMETABLES	32

typedef struct
{
	int id;
	List list;
	Color color;
} Timetable;

List init_from_file(int id, const char* path);

int load_timetables(Timetable timetables[], const char* path);

#endif // LOADER_H_
