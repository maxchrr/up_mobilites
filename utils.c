/*
 * Fonctions utilitaires
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */

 #include <stdlib.h>
 #include <time.h>
 #include <stdio.h>
 #include <string.h>
 #include "utils.h"
 #include "api.h"
 
 int rand_range(int min, int max)
{
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL) + clock());
        seeded = 1;
    }
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

struct Date rand_date(int start_year, int end_year)
{
    int year  = rand_range(start_year, end_year);
    int month = rand_range(1, 12);
    int day   = rand_range(1,
        (month == 2)                                   ? 28 :
        (month == 4 || month == 6 || month == 9 ||
         month == 11)                                  ? 30 :
                                                         31
    );
    return (struct Date){ day, month, year };
}

/*copyright (c) 2025 Emilio Decaix-Masiani. All Rights Reserved lol.*/
/* charge stations, routes et lignes */
int load_data(const char* filename,
	struct Bus_Stop*** sts, int* n_sts,
	struct Bus_Route*** rts, int* n_rts,
	struct Bus_Line*** bls, int* n_bls)
{
FILE* f = fopen(filename, "r");
if (!f) {
return -1;
}

int    i;
int    id;
int    x;
int    y;
int    pr;
int    d;
int    m;
int    yv;
char   name[30];

/* --- Stations --- */
if (fscanf(f, "%d", n_sts) != 1) {
fclose(f);
return -1;
}
*sts = malloc(*n_sts * sizeof(struct Bus_Stop*));

i = 0;
while (i < *n_sts) {
if (fscanf(f, "%d %29s %d %d %d %d %d %d",
		 &id, name, &x, &y, &pr, &d, &m, &yv) != 8) {
  fclose(f);
  return -1;
}
(*sts)[i] = create_bs(id, name, x, y);
bs_setmaint_price((*sts)[i], pr);
bs_setlast_maint_date((*sts)[i], (struct Date){ d, m, yv });
i++;
}

/* --- Routes --- */
if (fscanf(f, "%d", n_rts) != 1) {
fclose(f);
return -1;
}
*rts = malloc(*n_rts * sizeof(struct Bus_Route*));

i = 0;
while (i < *n_rts) {
int lid;
int dep_id;
int arr_id;
struct Bus_Stop* sdep = NULL;
struct Bus_Stop* sarr = NULL;

if (fscanf(f, "%d %d %d", &lid, &dep_id, &arr_id) != 3) {
  fclose(f);
  return -1;
}

int j = 0;
while (j < *n_sts) {
  if (bs_getid((*sts)[j]) == dep_id) {
	  sdep = (*sts)[j];
  }
  if (bs_getid((*sts)[j]) == arr_id) {
	  sarr = (*sts)[j];
  }
  j++;
}

int dx   = bs_getposx(sdep) - bs_getposx(sarr);
int dy   = bs_getposy(sdep) - bs_getposy(sarr);
int dist = abs(dx) + abs(dy);

(*rts)[i] = create_br(lid, sdep, sarr, dist, dist);
i++;
}

/* --- Lignes (1 ligne = 1 route) --- */
if (fscanf(f, "%d", n_bls) != 1) {
fclose(f);
return -1;
}
*bls = malloc(*n_bls * sizeof(struct Bus_Line*));

i = 0;
while (i < *n_bls) {
int idx;
if (fscanf(f, "%d", &idx) != 1) {
  fclose(f);
  return -1;
}
(*bls)[i] = create_bl((*rts)[idx]);
i++;
}

fclose(f);
return 0;
}

/* écrit stations, routes et lignes */
int save_data(const char* filename,
	struct Bus_Stop** sts, int n_sts,
	struct Bus_Route** rts, int n_rts,
	struct Bus_Line** bls, int n_bls)
{
FILE* f = fopen(filename, "w");
if (!f) {
return -1;
}

int         i;
struct Date dt;
struct Bus_Stop*   bs;
struct Bus_Route*  br;

/* --- Stations --- */
fprintf(f, "%d\n", n_sts);
i = 0;
while (i < n_sts) {
bs = sts[i];
dt = bs_getlast_maint_date(bs);
fprintf(f,
  "%d %s %d %d %d %02d %02d %04d\n",
  bs_getid(bs),
  bs_getname(bs),
  bs_getposx(bs),
  bs_getposy(bs),
  bs_getmaint_price(bs),
  dt.day, dt.month, dt.year
);
i++;
}

/* --- Routes --- */
fprintf(f, "%d\n", n_rts);
i = 0;
while (i < n_rts) {
br = rts[i];
fprintf(f, "%d %d %d\n",
  br_getbl_id(br),
  bs_getid(br_getdeparture(br)),
  bs_getid(br_getarrival(br))
);
i++;
}

/* --- Lignes --- */
fprintf(f, "%d\n", n_bls);
i = 0;
while (i < n_bls) {
fprintf(f, "%d\n", i);
i++;
}

fclose(f);
return 0;
}
