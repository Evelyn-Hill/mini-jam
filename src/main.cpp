#include <iostream>
#include <raylib.h>
#include <raygui.h>
#include "Entity.hpp"
#include "Global.hpp"
#include "TextureAtlas.hpp"

void Update();
void Draw();
void DrawRect(Entity* e);

static TextureAtlas ta;

int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(1280, 720, "Hello, World!");
	
	l->Info("Hello, Minijam!");

	ta.TALoadTexture("conveyorbelt");

	GetRectangleEntity({20, 20}, {20, 20}, DrawRect);

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
	
	DrawTexture(ta.GetTexture("conveyorbelt"), 0, 0, WHITE);
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
