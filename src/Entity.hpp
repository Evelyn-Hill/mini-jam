#pragma once
#include "Types.hpp"
#include <raymath.h>
#include "Global.hpp"

struct Entity;

enum AnimationState : u8 {
	DONE,
	PAUSED,
	PLAYING,
};

typedef void (*Render)(Entity* e);

struct Entity {
	vec2 position;
	vec2 size;
	vec2 startingPosition;
	vec2 destination;
	float animationDuration;
	float animationTimer;
	AnimationState animationState;
	Render renderMethod;
	int index;

	void Lerp(float delta) {
		if (animationState == AnimationState::DONE) {
			return;
		}

		if (animationState == AnimationState::PAUSED) {
			animationTimer += delta;
		}

		if (animationTimer < animationDuration) {
			animationTimer += delta;
			float alpha = Clamp(animationTimer / animationDuration, 0, 1);
			position = Vector2Lerp(startingPosition, destination, alpha);
		}
	}
};


Entity* GetRectangleEntity(vec2 pos, vec2 size, Render r) {
	
	Entity* e = new Entity {
		pos,
		size,
		pos,
		{0, 0},
		0.0,
		0.0,
		AnimationState::PAUSED,
		r
	};
	
	e->index = g->entities.size() - 1;
	g->entities.push_back(e);
	return e;
}

