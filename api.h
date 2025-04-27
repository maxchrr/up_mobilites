/*
 * Public API
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#ifndef API_H_
#define API_H_

/* Déclarations opaques des structures (encapsulation -> propriétées cachées) */
struct Bus_Stop;
struct Bus_Route;
struct Bus_Line;
struct Bus;

struct Node;
typedef struct Node* List;

typedef enum { BUS_LINE_INVALID, BUS_LINE_STOP, BUS_LINE_ROUTE } Bus_Line_Type;
typedef enum { DEP_TO_ARR, ARR_TO_DEP } Bus_Direction;

/* Méthodes sur les entités */
struct Bus_Stop*  create_bs(int id, char* name, int posx, int posy);
void              free_bs(struct Bus_Stop* bs);

struct Bus_Route*  create_br(int bl_id, struct Bus_Stop* departure, struct Bus_Stop* arrival, int distance_due, int time_due);
void               free_br(struct Bus_Route* br);

struct Bus_Line*  create_bl(struct Bus_Route* br);
void              print_bl(struct Bus_Line* bl, int indent);
void              free_bl(struct Bus_Line* bl);

struct Bus*  create_bus(int id, List start);
void        free_bus(struct Bus* bus);

/* Accesseur */
int          bs_getid(struct Bus_Stop* bs);
char*        bs_getname(struct Bus_Stop* bs);
int          bs_getposx(struct Bus_Stop* bs);
int          bs_getposy(struct Bus_Stop* bs);
int          bs_getmaint_price(struct Bus_Stop* bs);
struct Date  bs_getlast_maint_date(struct Bus_Stop* bs);

int               br_getbl_id(struct Bus_Route* br);
struct Bus_Stop*  br_getdeparture(struct Bus_Route* br);
struct Bus_Stop*  br_getarrival(struct Bus_Route* br);
int               br_getdistance_due(struct Bus_Route* br);
int               br_gettime_due(struct Bus_Route* br);

Bus_Line_Type  bl_gettype(struct Bus_Line*);

int            bus_getid(struct Bus* bus);
int            bus_getposx(struct Bus* bus);
int            bus_getposy(struct Bus* bus);
int            bus_getbl_id(struct Bus* bus);
List           bus_getpos_in_list(struct Bus* bus);
Bus_Direction  bus_getdirection(struct Bus* bus);

struct Node*  bs_getnext(List l);
struct Node*  bs_getprev(List l);
struct Node*  br_getnext(List l);
struct Node*  br_getprev(List l);
int           list_getposx(List l);
int           list_getposy(List l);

/* Mutateur */
void  bs_setmaint_price(struct Bus_Stop* bs, int value);
void  bs_setlast_maint_date(struct Bus_Stop* bs, struct Date date);

void  bus_setposx(struct Bus* bus, int posx);
void  bus_setposy(struct Bus* bus, int posy);
void  bus_setbl_id(struct Bus* bus, int bl_id);
void  bus_setpos_in_list(struct Bus* bus, List bl);
void  bus_setdirection(struct Bus* bus, Bus_Direction direction);
void  bus_setbus_on_bl(struct Bus* bus, List bl, Bus_Direction direction);

#endif // API_H_
