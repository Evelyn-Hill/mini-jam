#pragma once
#include "Types.hpp"
#include <raymath.h>

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

