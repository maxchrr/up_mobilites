/*
 * Fonctions utilitaires
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdlib.h>
#include <time.h>
#include "utils.h"

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
