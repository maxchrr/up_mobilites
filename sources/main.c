/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include "api.h"
#include "bus.h"
#include "list.h"
#include "raylib.h"
#include "ui.h"

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	800
#define WINDOW_TITLE	"UPmobilites"

List init_bl(int id, const char* names[], int positions[][2], int count)
{
	List new_bl;
	init_list(&new_bl);
	BusStation* cs = NULL;
	BusStation* ps = NULL;
	for (int i=0; i<count; ++i)
	{
		cs = create_bs(i+1, names[i], positions[i][0], positions[i][1]);
		if (i > 0)
		{
			BusRoute* r = create_br(id, ps, cs);
			new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
		}
		new_bl = insert_at_tail(new_bl, open_entity(STATION, cs));
		ps = cs;
	}
	return new_bl;
}

List create_bl1(int id)
{
	const char* names[] = {
		"Autoroute", "Rivière", "Centre ville", "Université", "Ville Sud"
	};
	int positions[][2] = {
		{100, 100}, {300, 100}, {500, 100}, {700, 100}, {900, 100}
	};
	return init_bl(id, names, positions, 5);
}

List create_bl2(int id)
{
	const char* names[] = {
		"Maison Royale", "Place du marché", "Gare", "Téléport"
	};
	int positions[][2] = {
		{400, 0}, {400, 150}, {400, 300}, {400, 450}
	};
	return init_bl(id, names, positions, 4);
}

List create_bl3(int id)
{
	const char* names[] = {
		"Zone Industrielle", "Déchetterie", "Ateliers Nord", "Quartier Est", "Centrale", "Parc Logistique", "Cimetière", "Centre Hospitalier"
	};
	int positions[][2] = {
		{220, -150}, {220, -50}, {220, 50}, {220, 150}, {220, 250}, {100, 250}, {100, 350}, {100, 450}
	};
	return init_bl(id, names, positions, 8);
}

List create_bl4(int id)
{
	const char* names[] = {
		"Zone Nord", "Piscine", "Hôtel de Ville", "Musée", "Stade"
	};
	int positions[][2] = {
		{0, 200}, {250, 200}, {500, 200}, {750, 200}, {1000, 200}
	};
	return init_bl(id, names, positions, 5);
}

int main(void)
{
	List bl1 = create_bl1(101);
	List bl2 = create_bl2(102);
	List bl3 = create_bl3(103);
	List bl4 = create_bl4(104);
	BusPtr bus1 = init_bus(1, bl1); // Bus sur ligne 1
	BusPtr bus2 = init_bus(2, bl2); // Bus sur ligne 2
	BusPtr bus3 = init_bus(3, bl3); // Bus sur ligne 3
	BusPtr bus4 = init_bus(4, bl4); // Bus sur ligne 4
	print_list(bl1);
	print_list(bl2);
	print_list(bl3);
	print_list(bl4);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Font font = LoadFontEx("vendor/Luciole-Regular.ttf", 18, NULL, 255);

	ClearBackground(RAYWHITE);
	draw_bus(bus1, YELLOW);
	draw_bus(bus2, ORANGE);
	draw_bus(bus3, GOLD);
	draw_bus(bus4, PINK);
	int incx1 = 0, incy1 = 0;
	int incx2 = 0, incy2 = 0;
	int incx3 = 0, incy3 = 0;
	int incx4 = 0, incy4 = 0;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		draw_bl(bl1, font, DARKGREEN);
		draw_bl(bl2, font, DARKBLUE);
		draw_bl(bl3, font, DARKPURPLE);
		draw_bl(bl4, font, DARKBROWN);

		draw_bus(bus1, YELLOW);
		bus_travel(bus1, bus_getdirection(bus1), &incx1, &incy1);
		draw_bus(bus2, ORANGE);
		bus_travel(bus2, bus_getdirection(bus2), &incx2, &incy2);
		draw_bus(bus3, GOLD);
		bus_travel(bus3, bus_getdirection(bus3), &incx3, &incy3);
		draw_bus(bus4, PINK);
		bus_travel(bus4, bus_getdirection(bus4), &incx4, &incy4);

		EndDrawing();
	}

	UnloadFont(font);
	CloseWindow();
	destroy_bus(bus1);
	destroy_bus(bus2);
	destroy_list(bl1);
	destroy_list(bl2);
	destroy_list(bl3);
	destroy_list(bl4);
	return 0;
}
