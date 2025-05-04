/*
 * Module utilitaires
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
