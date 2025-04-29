/*
 * Fonctions utilitaires
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "api.h"
#include "list.h" 

int rand_range(int min, int max)
{
	static int seeded = 0;
	if (!seeded)
	{
		srand(time(NULL) + clock());  // Génération pseudo-aléatoire
		seeded = 1;
	}
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

Date rand_date(int start_year, int end_year)
{
	int year = rand_range(start_year,end_year);
	int month = rand_range(1,12);
	int day = rand_range(1, (month == 2) ? (((year % 400 == 0) || (year % 4 == 0 && !(year % 100 == 0))) ? 29 : 28) : ((month == 4 || month == 6 || month == 9 || month == 11) ? 30 : 31));
	Date date;
	date.year = year;
	date.month = month;
	date.day = day;
	return date;
}

List import_stations_from_ini(const char* filename, int* bl_id_out)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    List station_list = NULL;
    char line[512]; // large pour tous lire si beaucoup d'info
    bool first_line_read = false;

    while (fgets(line, sizeof(line), file))
    {
        // on regarde pas les commentaires ( au cas ou )
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        // on enlève les saut de ligne
        line[strcspn(line, "\r\n")] = 0;

        if (!first_line_read)
        {
            *bl_id_out = atoi(line);
            first_line_read = true;
            continue;
        }

        int id = 0, posx = 0, posy = 0;
        char name[128] = {0};

        // Lire l'id
        char* p = line;
        if (sscanf(p, "%d", &id) != 1)
        {
            printf("Erreur lecture id sur la ligne : %s\n", line);
            continue;
        }

        // Avancer après l'id
        while (*p && (*p == ' ' || *p == '\t' || (*p >= '0' && *p <= '9')))
            p++;

        char* end = p;
        // Chercher la position où posx commence
        while (*end)
        {
            if ((*end >= '0' && *end <= '9') && 
                (end == p || *(end-1) == ' ' || *(end-1) == '\t'))
            {
                // Vérifier qu'il reste deux entiers
                int dummy_x, dummy_y;
                if (sscanf(end, "%d %d", &dummy_x, &dummy_y) == 2)
                    break;
            }
            end++;
        }

        // Copier le nom entre p et end
        int len = end - p;
        if (len <= 0 || len >= sizeof(name))
        {
            printf("Erreur lecture nom sur la ligne : %s\n", line);
            continue;
        }
        strncpy(name, p, len);
        name[len] = '\0';

        // Nettoyer les espaces de fin du nom
        for (int i = strlen(name) - 1; i >= 0 && (name[i] == ' ' || name[i] == '\t'); i--)
            name[i] = '\0';

        // Lire posx et posy
        if (sscanf(end, "%d %d", &posx, &posy) != 2)
        {
            printf("Erreur lecture posx/posy sur la ligne : %s\n", line);
            continue;
        }

        // Créer la station
        BusStation* bs = create_bs(id, name, posx, posy);
        if (!bs)
        {
            printf("Erreur allocation station pour : %s\n", line);
            continue;
        }

        BusEntity* entity = open_entity(1, bs);
        station_list = insert_at_tail(station_list, entity);
    }

    fclose(file);
    return station_list;
}