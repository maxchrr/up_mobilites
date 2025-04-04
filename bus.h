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
	Bus_Line_Object_Type bus_stop_or_line;

	/* Type représentant un arrêt */
	int id_bus_stop;
	char name[32];
	int pos_x, pos_y;

	/* Type représentant un tronçon */
	int id_bus_line;
	struct Bus_Line_Object* departure;
	struct Bus_Line_Object* arrival;
	int weight_distance;         // Coût en temps de parcours (secondes)
	int weight_time;             // Coût en distance (mètres)
	int distance_due, time_due;  // Algorithme de Dijkstra

	/* Pour les tris */
	int maintenance_price;
	Maintenance_Date maintenance_date;
} Bus_Line_Object;

#endif // BUS_H_
