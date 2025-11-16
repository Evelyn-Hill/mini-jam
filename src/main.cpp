#include <iostream>
#include <raylib.h>
#include <raygui.h>
#include "Entity.hpp"
#include "Global.hpp"
#include "TextureAtlas.hpp"

void Update();
void Draw();
void DrawRect(Entity* e);

const int POINT_LEN = 7;

static TextureAtlas ta;
static vec2 points[POINT_LEN] {
	{ -50, 350 },
	{200, 350},
	{900, 350},
	{1500, 350},
	{-50, 600},
	{700, 600},
	{1150, 710},
};

int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(1280, 720, "Hello, World!");
	
	l->Info("Hello, Minijam!");

	ta.TALoadTexture("conveyorbelt");

	GetRectangleEntity(points[0], {20, 20}, DrawRect, AnimationState::PLAYING);

	while (!WindowShouldClose()) {
		Update();
		Draw();
	}

	CloseWindow();
}


void Update() {
	for (Entity* e : g->entities) {
		e->Lerp(GetFrameTime(), points, POINT_LEN);
	}

}


void Draw() {
	BeginDrawing();

	ClearBackground(BLACK);
	
	DrawTexture(ta.GetTexture("conveyorbelt"), 0, 0, WHITE);
	for (Entity* e : g->entities) {
		if (e->dead) {
			delete g->entities[e->index];
			continue;
		}

		if (!e->visible) {
			continue;
		}

		e->renderMethod(e);
	}

	EndDrawing();
}


void DrawRect(Entity* e) { 		

	DrawRectangleRec(
		Rectangle {
			e->position.x - e->size.x / 2,
			e->position.y - e->size.y / 2,
			e->size.x,
			e->size.y
		},
		RED
	);
}
