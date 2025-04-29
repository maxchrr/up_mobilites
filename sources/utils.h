/*
 * Fonctions utilitaires
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef UTILS_H_
#define UTILS_H_

int rand_range(int min, int max);

typedef struct
{
	int day;
	int month;
	int year;
} Date;

Date rand_date(int start_year, int end_year);

#endif // UTILS_H_
