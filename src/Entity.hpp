#pragma once
#include "Types.hpp"
#include <raymath.h>
#include "Global.hpp"
#include "raylib.h"
#include "Rhythm.hpp"

struct Entity;

enum AnimationState : u8 {
	DONE,
	PAUSED,
	PLAYING,
};
enum EntityType : u8 {
	BOMB,
	OTHER,
	SUPERVISOR,
};

typedef void (*Render)(Entity* e);
typedef void (*Mimic)(Pattern* p);
typedef void (*Listen)(Pattern* p);

const int POINT_LEN = 7;

struct PointPair {
	int a;
	int b;
};

float waitMultipliers[POINT_LEN] = {
	0, 2, 4, 0, 2, 4, 0
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

bool skip[POINT_LEN] = {
	false, false, false, true, false, false, false
};

int mimicPoint = 2;
int playerPoint = 5;

vec2 bezierControlPoint = { 1000, 200 };
int bezierPoint = 5;
int bezierDuration = 1;

struct Entity {
	vec2 position;
	vec2 size;
	vec2 startingPosition;
	vec2 destination;
	float animationDuration = secondsPerBeat(g->tempo);
	float animationTimer = 0;
	float waitTime = 0; 
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
	int waitMultipler = 0;
	
	int lastEighth = 0;
	int currentEighth = 0;

	int lastQuarter = 0;
	int currentQuarter = 0;
	
	int lastHalf = 0;
	int currentHalf = 0;

	int lastWhole = 0;
	int currentWhole = 0;

	bool playedSound = false;

	int pickupIntervalBeats = 1;
	int pickupCount = 0;
	
	Pattern* pattern;
	Mimic mimic;
	Listen listen;

	bool mimicd = false;
	bool listened = false;

	void Lerp(float delta) {
		// Replace this 1 with the eventual beat time.
		waitTime = animationDuration * waitMultipliers[lastVisited];
		destination = points[lastVisited + 1];

		if (animationState == AnimationState::DONE) {
			return;
		}

		if (lastVisited == POINT_LEN - 1) {
			animationState = AnimationState::DONE;
			visible = false;
			dead = true;
			return;
		}

		if (skip[lastVisited] == true) {
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

			if (lastVisited == mimicPoint && !mimicd) {
        pattern->time = 0;
				g->currentPattern = pattern;
				g->state = PlaybackState::MIMIC;
				mimicd = true;
			}

			if (lastVisited == playerPoint && !listened) {
        pattern->time = 0;
				g->currentPattern = pattern;
				g->state = PlaybackState::LISTEN;
				listened = true;
			}
		}
	}

	void PollBeats() {
		if (animationState == AnimationState::DONE) {return;}

		currentEighth = getBeat(g->music, EIGHTH, g->tempo).beatNumber;
		currentQuarter = getBeat(g->music, QUARTER, g->tempo).beatNumber;
		currentHalf = getBeat(g->music, HALF, g->tempo).beatNumber;
		currentWhole = getBeat(g->music, WHOLE, g->tempo).beatNumber;

		if (lastEighth < currentEighth) {
			OnEighth();
			lastEighth = currentEighth;
		}

		if (lastQuarter < currentQuarter) {
			OnQuarter();
			lastQuarter = currentQuarter;
		}

		if (lastHalf < currentHalf) {
			OnHalf();
			lastHalf = currentHalf;
		}

		if (lastWhole < currentWhole) {
			OnWhole();
			lastWhole = currentWhole;
		}
	}
	
	void OnEighth() {
	}


	void OnQuarter() {
		pickupCount++;
	}


	void OnHalf() {
	}


	void OnWhole() {
	}

};

Entity* GetRectangleEntity(vec2 pos, vec2 size, Render r, AnimationState defaultAnimState) {	
	Entity* e = new Entity {
		pos,
		size,
		pos,
		{0, 0},
		secondsPerBeat(g->tempo),
		0.0,
		secondsPerBeat(g->tempo),
		defaultAnimState,
		r
	};

	
	g->entities.push_back(e);
	e->index = g->entities.size() - 1;
	return e;
}

Entity* GetSpriteEntity(vec2 pos, Render r, AnimationState defaultAnimState, Texture2D tex, float rotSpeed = 0, int drawLayer = 0) {
	Entity* e = new Entity();
	e->position = pos;
	e->size = vec2(tex.width, tex.height);
	e->startingPosition = pos;	
	e->animationDuration = secondsPerBeat(g->tempo);
	e->animationState = defaultAnimState;
	e->renderMethod = r;
	e->texture = tex;
	e->rotSpeed = rotSpeed;
	e->drawLayer = drawLayer;
	g->entities.push_back(e);
	e->index = g->entities.size() - 1;
	return e;
}

void DeleteEntity(Entity* e) {
	for (int i = 0; i < g->entities.size(); i++) {
		if (g->entities[i] == e) {
			g->entities.erase(g->entities.begin() + i);
		}
	}
}
