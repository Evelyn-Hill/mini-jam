#include <iostream>
#include <raylib.h>
#include <raygui.h>
#include "Global.hpp"


int main() {
	InitWindow(1280, 720, "Hello, World!");
	
	l->Info(g->name);
	
	while (!WindowShouldClose()) {
		ClearBackground(WHITE);
	}

	CloseWindow();
}
