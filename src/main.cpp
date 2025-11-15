#include <iostream>
#include <raylib.h>
#include <raygui.h>
#include "Entity.hpp"
#include "Global.hpp"


void Update();
void Draw();
void DrawRect(Entity* e);

int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(1280, 720, "Hello, World!");
	
	l->Info("Hello, Minijam!");

	
	Entity* e = new Entity {
		{50, 50},
		{20, 20},
		{10, 10},
		{50, 50},
		5.0,
		0.0,
		AnimationState::PLAYING,
		DrawRect,
	};

	g->entities.push_back(e);

	while (!WindowShouldClose()) {
		Update();
		Draw();
	}

	CloseWindow();
}



void Update() {

	for (Entity* e : g->entities) {
		e->Lerp(GetFrameTime());
	}

}


void Draw() {
	BeginDrawing();

	ClearBackground(BLACK);

	for (Entity* e : g->entities) {
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
