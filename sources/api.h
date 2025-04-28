/*
 * Public API
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef API_H_
#define API_H_

#include "utils.h"

/* Déclarations opaques des structures (encapsulation -> propriétées cachées) */
typedef struct BusStation BusStation;
typedef struct BusRoute BusRoute;

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
BusStation*         create_bs(int id, char* name, int posx, int posy);
static void         print_bs(BusStation* bs, int indent);
void                destroy_bs(BusStation* bs);

BusRoute*          create_br(int bl_id, BusStation* departure, BusStation* arrival, int distance_due, int time_due);
static void        print_br(BusRoute* br, int indent);
void               destroy_br(BusRoute* br);

BusEntity*  open_entity(int is_station, void* data);
void        print_entity(BusEntity* obj, int indent);
void        close_entity(BusEntity* obj);

/* Accesseur */
int          bs_getid(BusStation* bs);
char*        bs_getname(BusStation* bs);
int          bs_getposx(BusStation* bs);
int          bs_getposy(BusStation* bs);
int          bs_getmaint_price(BusStation* bs);
struct Date  bs_getlast_maint_date(BusStation* bs);

int               br_getbl_id(BusRoute* br);
BusStation*  br_getdeparture(BusRoute* br);
BusStation*  br_getarrival(BusRoute* br);
int               br_getdistance_due(BusRoute* br);
int               br_gettime_due(BusRoute* br);

/* Mutateur */
void  bs_setmaint_price(BusStation* bs, int value);
void  bs_setlast_maint_date(BusStation* bs, struct Date date);

#endif // API_H_
