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
	float animationDuration = 1.0;
	float animationTimer = 0;
	float waitTime = 0; 
	AnimationState animationState;
	Render renderMethod;
	int index;
	int lastVisited;
	float waitTimer = 0;
	bool visible = true;
	bool dead = false;
	

	void Lerp(float delta, vec2 points[], int pointSize) {
		if (animationState == AnimationState::DONE) {
			return;
		}

		if (lastVisited == pointSize - 1) {
			animationState = AnimationState::DONE;
			l->Info("Final Point");
			visible = false;
			dead = true;
			return;
		}

		if (animationState == AnimationState::PAUSED) {
			waitTimer += delta;
			if (waitTimer >= waitTime) {
				animationState = AnimationState::PLAYING;
				waitTimer = 0;
			}
			return;
		}

		if (animationTimer < animationDuration) {
			destination = points[lastVisited + 1];
			animationTimer += delta;
			float alpha = Clamp(animationTimer / animationDuration, 0, 1);
			position = Vector2Lerp(startingPosition, destination, alpha);
		} else {
			animationState = AnimationState::PAUSED;
			animationTimer = 0;
			startingPosition = destination;
			lastVisited++;
		}
	}
};


Entity* GetRectangleEntity(vec2 pos, vec2 size, Render r, AnimationState defaultAnimState) {	
	Entity* e = new Entity {
		pos,
		size,
		pos,
		{0, 0},
		3.0,
		0.0,
		1.0,
		defaultAnimState,
		r
	};

	e->lastVisited = 0;
	
	g->entities.push_back(e);
	e->index = g->entities.size() - 1;
	return e;
}

