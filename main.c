#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	400

typedef struct
{
	Vector2 position;
	const char* name;
} Station;

typedef struct
{
	int currentStation;
	int nextStation;
	float progress;
	float speed;
	Color color;
} Bus;

int main(void)
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "UPmobilites");
	SetTargetFPS(60);

	Station stations[] = {
		{{100, 200}, "Gare d'Esbly"},
		{{200, 200}, "Collège Louis Braille"},
		{{300, 200}, "Vignes Rouges"},
		{{400, 200}, "Cents Arpents"},
		{{500, 200}, "Trois Ormes"},
		{{600, 200}, "Hôtels du Val de France"},
		{{700, 200}, "Hôtels Disneyland"},
		{{800, 200}, "Marne-la-Vallée Chessy"},
		{{900, 200}, "Centre de Secours"},
		{{1000, 200}, "Val d'Europe"},
		{{1100, 200}, "Centre Hospitalier"}
	};
	int stationCount = sizeof(stations) / sizeof(Station);

	Bus bus = {0, 1, 0.0f, 0.2f, RED};

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		bus.progress += bus.speed * deltaTime;
		if (bus.progress >= 1.0f)
		{
			bus.currentStation++;
			if (bus.currentStation >= stationCount - 1)
				bus.currentStation = 0;
			bus.nextStation = bus.currentStation + 1;
			bus.progress = 0.0f;
		}

		Vector2 posStart = stations[bus.currentStation].position;
		Vector2 posEnd = stations[bus.nextStation].position;
		Vector2 busPos = {
			posStart.x + (posEnd.x - posStart.x) * bus.progress,
			posStart.y + (posEnd.y - posStart.y) * bus.progress
		};

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawLineEx((Vector2){100, 200}, (Vector2){1100, 200}, 10.0f, PURPLE);

		for (int i = 0; i < stationCount; i++) {
			DrawCircleV(stations[i].position, 8, DARKPURPLE);
			DrawText(stations[i].name, stations[i].position.x - 50, stations[i].position.y - 50, 10, BLACK);
		}

		DrawCircleV(busPos, 10, bus.color);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
