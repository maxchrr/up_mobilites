/*
 * Fonctions utilitaires
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "api.h" 

int rand_range(int min, int max);

struct Date {
    int day;
    int month;
    int year;
};

struct Date rand_date(int start_year, int end_year);

/* lecture/écriture de x.data */
int load_data(const char* filename,
              struct Bus_Stop*** sts, int* n_sts,
              struct Bus_Route*** rts, int* n_rts,
              struct Bus_Line*** bls, int* n_bls);

int save_data(const char* filename,
              struct Bus_Stop** sts, int n_sts,
              struct Bus_Route** rts, int n_rts,
              struct Bus_Line** bls, int n_bls);

#endif // UTILS_H_
