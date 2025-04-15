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
	int bus_stop_id;
	char name[32];
	int pos_x, pos_y;

	/* Type représentant un tronçon */
	int bus_route_id;
	struct Bus_Line_Object* departure;
	struct Bus_Line_Object* arrival;
	int distance_due; // Coût en distance (mètres)
	int time_due;     // Coût en temps de parcours (secondes)
	int weight_distance, weight_time; // Algorithme de Dijkstra

	/* Pour les tris */
	int maintenance_price;
	Maintenance_Date maintenance_date;
} Bus_Line_Object;

struct Node
{
	Bus_Line_Object* data; // Data held by the node - pointer to integer.
	struct Node* next;
	struct Node* prev;
};
typedef struct Node* List_Bus_Line; // global variable - pointer to head node.

void print_bus_object(Bus_Line_Object* bus_line);

int get_bus_stop_id(Bus_Line_Object* bus_line);
char* get_bus_stop_name(Bus_Line_Object* bus_line);
int get_bus_stop_pos_x(Bus_Line_Object* bus_line);
int get_bus_stop_pos_y(Bus_Line_Object* bus_line);
int get_bus_route_id(Bus_Line_Object* bus_line);
Bus_Line_Object_Type get_type(Bus_Line_Object* bus_line);
int get_maintenance_price(Bus_Line_Object* bus_line);
Maintenance_Date get_maintenance_date(Bus_Line_Object* bus_line);

void set_maintenance_price(Bus_Line_Object* bus_line, int new_price);
void set_maintenance_date(Bus_Line_Object* bus_line, Maintenance_Date new_date);

typedef enum
{
	DEP_TO_ARR,
	ARR_TO_DEP
} Bus_Line_Direction;

struct Bus
{
	int bus_id;
	int pos_x, pos_y;
	int bus_line_id;
	List_Bus_Line bus_line_pos;
	Bus_Line_Direction direction;
};
typedef struct Bus* Bus_Object;

int get_bus_id(Bus_Object bus);
int get_bus_pos_x(Bus_Object bus);
int get_bus_pos_y(Bus_Object bus);
int get_bus_line_id(Bus_Object bus);
List_Bus_Line get_bus_line_pos(Bus_Object bus);
Bus_Line_Direction get_direction(Bus_Object bus);

void set_bus_pos_x(Bus_Object bus, int new_pos_x);
void set_bus_pos_y(Bus_Object bus, int new_pos_y);
void set_bus_line_id(Bus_Object bus, int new_bus_line);
void set_bus_line_pos(Bus_Object bus, List_Bus_Line bus_line);
void set_direction(Bus_Object bus, Bus_Line_Direction new_direction);

Bus_Object create_bus(int id, List_Bus_Line start);
Bus_Line_Object* create_stop(int id, char* name, int pos_x, int pos_y);
Bus_Line_Object* create_route(
	int id,
	Bus_Line_Object* departure,
	Bus_Line_Object* arrival,
	int distance_due,
	int time_due
);

void print_bus_line(List_Bus_Line l);

struct Node* get_next_stop(List_Bus_Line l);
struct Node* get_prev_stop(List_Bus_Line l);
struct Node* get_next_route(List_Bus_Line l);
struct Node* get_prev_route(List_Bus_Line l);
int get_pos_x_in_list(List_Bus_Line l);
int get_pos_y_in_list(List_Bus_Line l);

void set_bus_on_bus_line(Bus_Object bus, List_Bus_Line bus_line, Bus_Line_Direction direction);

#endif // BUS_H_
