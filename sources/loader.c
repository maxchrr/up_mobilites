/*
 * Lecteur de fichier
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include "globals.h"
#include "api.h"
#include "list.h"
#include "loader.h"

#define MY_PERROR(FMT,...) \
	fprintf(stderr, FMT ": %s\n", __VA_ARGS__, strerror(errno))

Timetables load_dir(const char* path)
{
	DIR* dir = opendir(path);
	if (!dir)
	{
		MY_PERROR("Unable to read directory %s\n", path);
		exit(EXIT_FAILURE);
	}

	Timetables timetables = {0};
	timetables.capacity = 100;  // Valeur assez grande pour éviter le redimensionnement
	timetables.ids  = calloc(timetables.capacity, sizeof(int));
	timetables.files = calloc(timetables.capacity, sizeof(char*));
	if (!timetables.ids || !timetables.files)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	char fullPath[MAX_BUFFER_SIZE];
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		const char* fileName = entry->d_name;
		bool is_hidden = fileName[0] == '.';
		char* is_lock = strstr(fileName, "~");
		char* is_swap = strstr(fileName, ".swp");
		char* is_backup = strstr(fileName, ".bak");
		if (!is_hidden && !is_lock && !is_swap && !is_backup)
		{
			const char* name = "timetable";
			size_t name_size = strlen(name);
			bool correct_name = strncmp(fileName, name, name_size) == 0;
			size_t fileName_size = strlen(fileName);
			bool correct_ext = strcmp(fileName + fileName_size - 4, ".ini") == 0;
			if (correct_name && correct_ext)
			{
				// Extraire l'identifiant
				int id;
				bool is_id = sscanf(fileName + name_size, "%d", &id) == 1;
				if (is_id)
				{
					// Construction du chemin du fichier
					snprintf(fullPath, sizeof(fullPath), "%s/%s", path, fileName);
					// Redimensionner si besoin (très coûteux en mémoire)
					if (timetables.count >= timetables.capacity)
					{
						timetables.capacity *= 2;
						int* newIds = realloc(timetables.ids, sizeof(int)*timetables.capacity);
						char** newFiles = realloc(timetables.files, sizeof(char*)*timetables.capacity);
						if (!newIds || !newFiles)
						{
							fprintf(stderr, "Memory allocation failed\n");
							exit(EXIT_FAILURE);
						}
						timetables.ids = newIds;
						timetables.files = newFiles;
					}
					timetables.ids[timetables.count] = id;
					timetables.files[timetables.count] = strdup(fullPath);
					if (!timetables.files[timetables.count])
					{
						fprintf(stderr, "strdup failed\n");
						exit(EXIT_FAILURE);
					}
					++timetables.count;
				}
			}
		}
	}

	closedir(dir);
	return timetables;
}

List read_file(const int bl_id, const char* path)
{
	FILE* file = fopen(path, "r");
	if (!file)
	{
		MY_PERROR("Unable to read file %s\n", path);
		exit(EXIT_FAILURE);
	}

	List new_bl; init_list(&new_bl);
	BusStation* prev_station = NULL;
	BusStation* curr_station = NULL;
	BusRoute* route = NULL;
	char bl_bsname[MAX_NAME_LEN];
	int bl_bsposx, bl_bsposy;
	int i = 1;
	char buffer[MAX_BUFFER_SIZE];
	int stationCount = 0;

	while (fgets(buffer, sizeof(buffer), file))
	{
		bool is_empty_line = buffer[0] == '\n' || buffer[0] == '\r';
		bool is_comment = buffer[0] == '#';
		if (!is_empty_line || !is_comment)
		{
			// Une ligne de fichier correct est de la forme : Foo,10,10
			char* format = "%31[^,],%d,%d\n";
			bool is_valid_line = sscanf(buffer, format, bl_bsname, &bl_bsposx, &bl_bsposy) == 3;
			if (is_valid_line)
			{
				curr_station = create_bs(++i, bl_bsname, bl_bsposx, bl_bsposy);
				if (!curr_station)
				{
					fprintf(stderr, "Erreur lors de la création de  BusStation\n");
					fclose(file);
					destroy_list(new_bl);
					exit(EXIT_FAILURE);
				}
				++stationCount;
				// Crée un chemin si pas un terminus
				if (prev_station != NULL)
				{
					route = create_br(bl_id, prev_station, curr_station);
					if (!route)
					{
						fprintf(stderr, "Erreur lors de la création de BusRoute\n");
						fclose(file);
						destroy_list(new_bl);
						exit(EXIT_FAILURE);
					}
					new_bl = insert_at_tail(new_bl, open_entity(ROUTE, route));
				}
				new_bl = insert_at_tail(new_bl, open_entity(STATION, curr_station));
				prev_station = curr_station;  // Station suivante
			}
		}
	}

	if (stationCount < 2)
	{
		fprintf(stderr, "Invalid file %s\n", path);
		destroy_list(new_bl);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	fclose(file);
	return new_bl;
}

void free_timetables(Timetables t)
{
	for (unsigned i=0; i<t.count; ++i) free(t.files[i]);
	free(t.ids);
	free(t.files);
	t.ids = NULL;
	t.files = NULL;
	t.count = 0;
	t.capacity = 0;
}
