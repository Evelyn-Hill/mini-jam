#include <iostream>
#include <raylib.h>
#include <raygui.h>
#include <unordered_map>
#include <utility>
#include "Entity.hpp"
#include "Global.hpp"
#include "TextureAtlas.hpp"

void Update();
void Draw();
void DrawRect(Entity* e);
void DrawTex(Entity* e);
void FlushEntities();


static TextureAtlas ta;


int main() {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(1280, 720, "Hello, World!");
	
	l->Info("Hello, Minijam!");

	ta.TALoadTexture("bgtest");
	ta.TALoadTexture("bomb1");

	GetSpriteEntity(points[0], DrawTex, AnimationState::PLAYING, ta.GetTexture("bomb1"));

	while (!WindowShouldClose()) {
		Update();
		Draw();
		FlushEntities();
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
	
	DrawTexture(ta.GetTexture("bgtest"), 0, 0, WHITE);
	for (Entity* e : g->entities) {
		if (!e->visible) {
			continue;
		}

		e->renderMethod(e);
	}

	EndDrawing();
}


void FlushEntities() {
	for (Entity* e : g->entities) {
		if (e->dead) {
			DeleteEntity(e);
			continue;
		}
	}
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

void DrawTex(Entity* e) {
	DrawTextureEx(e->texture, 
			   e->position - (e->size / 2), 

			   0, 
			   1, 
			   WHITE);
}
