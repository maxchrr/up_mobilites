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

List create_bl1(const int id)
{
	BusStation* d;
	BusStation* a;
	BusRoute* r;
	List new_bl;
	init_list(&new_bl);
	d = create_bs(1, "Autoroute", 100, 100);
	a = create_bs(2, "Rivière", 300, 100);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	d = create_bs(3, "Centre ville", 500, 100);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	a = create_bs(4, "Université", 700, 100);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	d = create_bs(5, "Ville Sud", 900, 100);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	return new_bl;
}

List create_bl2(const int id)
{
	BusStation* d;
	BusStation* a;
	BusRoute* r;
	List new_bl;
	init_list(&new_bl);
	d = create_bs(1, "Maison Royale", 400, 0);
	a = create_bs(2, "Place du marché", 400, 150);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	d = create_bs(3, "Gare", 400, 300);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	a = create_bs(4, "Téléport", 400, 450);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	return new_bl;
}

List create_bl3(const int id)
{
	BusStation* d;
	BusStation* a;
	BusRoute* r;
	List new_bl;
	init_list(&new_bl);
	d = create_bs(1, "Zone Industrielle", 220, -150);
	a = create_bs(2, "Déchetterie", 220, -50);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	d = create_bs(3, "Ateliers Nord", 220, 50);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	a = create_bs(4, "Quartier Est", 220, 150);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	d = create_bs(5, "Centrale", 220, 250);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	a = create_bs(6, "Parc Logistique", 100, 250);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	d = create_bs(7, "Cimetière", 100, 350);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, d));
	a = create_bs(8, "Centre Hospitalier", 100, 450);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(ROUTE, r));
	new_bl = insert_at_tail(new_bl, open_entity(STATION, a));
	return new_bl;
}

int main(void)
{
	List bl1 = create_bl1(101);
	List bl2 = create_bl2(102);
	List bl3 = create_bl3(103);
	BusPtr bus1 = init_bus(1, bl1); // Bus sur ligne 1
	BusPtr bus2 = init_bus(1, bl2); // Bus sur ligne 2
	print_list(bl1);
	print_list(bl2);
	print_list(bl3);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Font font = LoadFontEx("vendor/Luciole-Regular.ttf", 18, NULL, 255);

	ClearBackground(RAYWHITE);
	draw_bus(bus1, YELLOW);
	draw_bus(bus2, YELLOW);
	int incx1 = 0, incy1 = 0;
	int incx2 = 0, incy2 = 0;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		draw_bl(bl1, font, DARKGREEN);
		draw_bl(bl2, font, DARKBLUE);
		draw_bl(bl3, font, DARKPURPLE);

		draw_bus(bus1, YELLOW);
		bus_travel(bus1, bus_getdirection(bus1), &incx1, &incy1);
		draw_bus(bus2, ORANGE);
		bus_travel(bus2, bus_getdirection(bus2), &incx2, &incy2);

		EndDrawing();
	}

	UnloadFont(font);
	CloseWindow();
	destroy_bus(bus1);
	destroy_bus(bus2);
	destroy_list(bl1);
	destroy_list(bl2);
	destroy_list(bl3);
	return 0;
}
