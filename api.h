#ifndef API_H_
#define API_H_

/* Déclarations opaques des structures (encapsulation -> propriétées cachées) */
struct Bus_Stop;
struct Bus_Route;
struct Bus_Line;
struct Bus;

typedef enum { BUS_LINE_INVALID, BUS_LINE_STOP, BUS_LINE_ROUTE } Bus_Line_Type;
typedef enum { DEP_TO_ARR, ARR_TO_DEP } Bus_Direction;

/* Méthodes sur les entités */
struct Bus_Stop*  create_bs(int id, char* name, int pos_x, int pos_y);
void              free_bs(struct Bus_Stop* bs);

struct Bus_Route*  create_br(int bus_line_id, struct Bus_Stop* departure, struct Bus_Stop* arrival, int distance_due, int time_due);
void               free_br(struct Bus_Route* br);

struct Bus_Line*  create_bl(struct Bus_Route* br);
void              print_bl(struct Bus_Line* bl, int indent);
void              free_bl(struct Bus_Line* bl);

/* Accesseur */
int          bs_getid(struct Bus_Stop* bs);
char*        bs_getname(struct Bus_Stop* bs);
int          bs_getposx(struct Bus_Stop* bs);
int          bs_getposy(struct Bus_Stop* bs);
int          bs_getmaint_price(struct Bus_Stop* bs);
struct Date  bs_getlast_maint_date(struct Bus_Stop* bs);

int               br_getbus_line_id(struct Bus_Route* br);
struct Bus_Stop*  br_getdeparture(struct Bus_Route* br);
struct Bus_Stop*  br_getarrival(struct Bus_Route* br);
int               br_getdistance_due(struct Bus_Route* br);
int               br_gettime_tue(struct Bus_Route* br);

Bus_Line_Type  bl_gettype(struct Bus_Line*);

/* Mutateur */
void  bs_setmaint_price(struct Bus_Stop* bs, int value);
void  bs_setlast_maint_date(struct Bus_Stop* bs, struct Date date);

#endif // API_H_
