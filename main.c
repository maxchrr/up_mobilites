/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include "utils.h"

int main(void)
{
    List stations = NULL;
    int bus_line_id = 0;

    stations = import_stations_from_ini("../vendor/timetable.ini", &bus_line_id);

    printf("Ligne de bus lue depuis fichier : %d\n", bus_line_id);
    print_list(stations);
    destroy_list(stations);
    return 0;
}
