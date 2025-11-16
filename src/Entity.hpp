#pragma once
#include "Types.hpp"
#include <raymath.h>
#include "Global.hpp"
#include "raylib.h"

struct Entity;

enum AnimationState : u8 {
	DONE,
	PAUSED,
	PLAYING,
};

typedef void (*Render)(Entity* e);

const int POINT_LEN = 7;

float waitMultipliers[POINT_LEN] = {
	0, 1, 2, 0.1, 0.1, 2, 0.1
};

bool skip[POINT_LEN] = {
	false, false, false, true, false, false, false
};



vec2 points[POINT_LEN] {
	{ -300, 280 },
	{200, 280},
	{900, 280},
	{1700, 280},
	{-300, 520},
	{700, 520},
	{1150, 710},
};

vec2 bezierControlPoint = { 1000, 200 };
int bezierPoint = 5;
int bezierDuration = 1;



struct Entity {
	vec2 position;
	vec2 size;
	vec2 startingPosition;
	vec2 destination;
	float animationDuration = 1.0;
	float animationTimer = 0;
	int waitTime = 0; 
	AnimationState animationState;
	Render renderMethod;
	int index;
	int lastVisited = 0;
	float waitTimer = 0;
	bool visible = true;
	bool dead = false;
	Texture2D texture;
	float rotSpeed;
	int drawLayer = 0;

	

	void Lerp(float delta) {
		// Replace this 1 with the eventual beat time.
		waitTime = 1 * waitMultipliers[lastVisited];

		if (animationState == AnimationState::DONE) {
			return;
		}

		if (lastVisited == POINT_LEN - 1) {
			animationState = AnimationState::DONE;
			l->Info("Final Point");
			visible = false;
			dead = true;
			return;
		}


		if (skip[lastVisited] == true) {
			l->Info("skip");
			startingPosition = points[lastVisited + 1];
			animationTimer = 0;
			waitTimer = 0;
			lastVisited++;
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
			if (skip[lastVisited] == true) {
				destination = points[lastVisited];
			} else {
				destination = points[lastVisited + 1];
			}

			animationTimer += delta;
			if (lastVisited != bezierPoint) {
				float alpha = Clamp(animationTimer / animationDuration, 0, 1);
				position = Vector2Lerp(startingPosition, destination, alpha);
			} else {
				float alpha = Clamp(animationTimer / bezierDuration, 0, 1);
				position = GetSplinePointBezierQuad(startingPosition, bezierControlPoint, destination, alpha);
			}
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
		0,
		defaultAnimState,
		r
	};

	
	g->entities.push_back(e);
	e->index = g->entities.size() - 1;
	return e;
}

Entity* GetSpriteEntity(vec2 pos, Render r, AnimationState defaultAnimState, Texture2D tex, float rotSpeed = 0, int drawLayer = 0) {
	Entity* e = new Entity {
		pos,
		{0, 0},
		pos,
		{0, 0},
		3.0,
		0.0,
		0,
		defaultAnimState,
		r
	};

	e->texture = tex;
	e->size = vec2(tex.width, tex.height);
	e->rotSpeed = rotSpeed;
	e->drawLayer = drawLayer;

	g->entities.push_back(e);
	e->index = g->entities.size() - 1;
	return e;
}

Entity* GetRotatingBackgroundEntity(vec2 pos, Render r, float rotSpeed, Texture2D tex) {

};

void DeleteEntity(Entity* e) {
	for (int i = 0; i < g->entities.size(); i++) {
		if (g->entities[i] == e) {
			g->entities.erase(g->entities.begin() + i);
		}
	}
}

