/*
 * Ligne de Bus
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#pragma once

#include "list.h"

typedef List BusLine;  // Pour plus de lisibilité

int      bl_getcurrent_posx(BusLine l);
int      bl_getcurrent_posy(BusLine l);
BusLine  bl_getnext_bs(BusLine l);
BusLine  bl_getprev_bs(BusLine l);
BusLine  bl_getnext_br(BusLine l);
BusLine  bl_getprev_br(BusLine l);

void     bl_remove(BusLine bl);
BusLine  bl_concat(BusLine bl1, BusLine bl2);
