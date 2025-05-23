/*
 * Public API - Station et Route
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
#pragma once

#include "utils.h"

/* Déclarations opaques des structures (encapsulation -> propriétées cachées) */
typedef struct BusStation BusStation;
typedef struct BusRoute BusRoute;

typedef enum { INVALID, STATION, ROUTE } EntityType;

/* Entité global via union */
typedef struct BusEntity
{
	unsigned station : 1;
	unsigned route : 1;
	union
	{
		BusStation* bs;  // Pointeur sur une station
		BusRoute* br;    // Pointeur sur une route
	};
} BusEntity;

/* Méthodes sur les entités */
BusStation*  create_bs(int id, const char* name, int posx, int posy);
void         print_bs(const BusStation* bs);
void         destroy_bs(BusStation* bs);

BusRoute*  create_br(int bl_id, BusStation* departure, BusStation* arrival);
void       print_br(const BusRoute* br);
void       destroy_br(BusRoute* br);

BusEntity*  open_entity(EntityType type, void* data);
void        print_entity(const BusEntity* obj);
void        close_entity(BusEntity* obj);

/* Accesseur */
int          bs_getid(const BusStation* bs);
const char*  bs_getname(const BusStation* bs);
int          bs_getposx(const BusStation* bs);
int          bs_getposy(const BusStation* bs);
int          bs_getmaint_price(const BusStation* bs);
Date         bs_getlast_maint_date(const BusStation* bs);

int          br_getbl_id(const BusRoute* br);
BusStation*  br_getdeparture(const BusRoute* br);
BusStation*  br_getarrival(const BusRoute* br);
int          br_getdistance(const BusRoute* br);
int          br_gettime(const BusRoute* br);

EntityType  gettype(const BusEntity* obj);

/* Mutateur */
void  bs_setmaint_price(BusStation* bs, int value);
void  bs_setlast_maint_date(BusStation* bs, Date date);
