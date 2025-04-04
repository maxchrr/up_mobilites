#ifndef BUS_H_
#define BUS_H_

typedef enum
{
	BUS_STOP,
	BUS_ROUTE
} Bus_Line_Object_Type;

typedef struct
{
	int day;
	int month;
	int year;
} Maintenance_Date;

typedef struct
{
	Bus_Line_Object_Type bus_stop_or_route;

	/* Type représentant un arrêt */
	int id_bus_stop;
	char name[32];
	int pos_x, pos_y;

	/* Type représentant un tronçon */
	int id_bus_route;
	struct Bus_Line_Object* departure;
	struct Bus_Line_Object* arrival;
	int distance_due; // Coût en distance (mètres)
	int time_due;     // Coût en temps de parcours (secondes)
	int weight_distance, weight_time; // Algorithme de Dijkstra

	/* Pour les tris */
	int maintenance_price;
	Maintenance_Date maintenance_date;
} Bus_Line_Object;

void print_bus_line(Bus_Line_Object* bus_line);

int get_id_bus_stop(Bus_Line_Object* bus_line);
char* get_name(Bus_Line_Object* bus_line);
int get_pos_x(Bus_Line_Object* bus_line);
int get_pos_y(Bus_Line_Object* bus_line);
int get_id_bus_route(Bus_Line_Object* bus_line);
Bus_Line_Object_Type get_type(Bus_Line_Object* bus_line);

#endif // BUS_H_
