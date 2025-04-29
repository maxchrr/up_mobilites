/*
 * UPmobilites
 * Copyright (c) 2025 Max Charrier, Emilio Decaix-Massiani. All Rights Reserved.
 */
#include <stdio.h>
#include "api.h"
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
	new_bl = insert_at_tail(new_bl, open_entity(1, d));
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, a));
	d = create_bs(3, "Centre ville", 500, 100);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, d));
	a = create_bs(4, "Université", 700, 100);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, a));
	d = create_bs(5, "Ville Sud", 900, 100);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, d));
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
	new_bl = insert_at_tail(new_bl, open_entity(1, d));
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, a));
	d = create_bs(3, "Gare", 400, 300);
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, d));
	a = create_bs(4, "Téléport", 400, 450);
	r = create_br(id, d, a);
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	new_bl = insert_at_tail(new_bl, open_entity(1, a));
	r = create_br(id, a, d);
	new_bl = insert_at_tail(new_bl, open_entity(0, r));
	return new_bl;
}

int main(void)
{
	List bl1 = create_bl1(101);
	print_list(bl1);
	List bl2 = create_bl2(102);
	print_list(bl2);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(60);

	Font font = LoadFontEx("vendor/selawk.ttf", 36, NULL, 0);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		draw_bl(bl1, font, DARKGREEN);
		draw_bl(bl2, font, DARKBLUE);
		EndDrawing();
	}

	UnloadFont(font);
	CloseWindow();
	destroy_list(bl1);
	destroy_list(bl2);
	return 0;
}
