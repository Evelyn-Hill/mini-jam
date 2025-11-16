#include "Entity.hpp"
#include "Global.hpp"
#include "Level.hpp"
#include "TextureAtlas.hpp"
#include "Scoring.hpp"
#include <iostream>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <utility>

void Update();
void Draw();
void DrawRect(Entity *e);
void DrawTex(Entity *e);
void FlushEntities();
void DrawRotTex(vec2 pos, Texture2D tex, float time, float rotSpeed);

static TextureAtlas ta;

int main() {
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(1280, 720, "Hello, World!");
  InitAudioDevice();

  l->Info("Hello, Minijam!");
  g->tempo = 180;

  g->music = LoadMusicStream("assets/save_it_redd.mp3");

  g->level = new Level();
  levelAppend(g->level, 1);
  l->Info("created level");

  int totalSongBeats =
      round(GetMusicTimeLength(g->music) / secondsPerBeat(g->tempo));
  l->Info("song has ", totalSongBeats, " beats");
  int measures = (totalSongBeats - 1) / 4;
  l->Info("song has ", measures, " measures");

  Pattern fourQuarterNotes = {
      (Beat){.count = 1, .subdivision = QUARTER},
      (Beat){.count = 1, .subdivision = QUARTER},
      (Beat){.count = 1, .subdivision = QUARTER},
      (Beat){.count = 1, .subdivision = QUARTER},
  };
  l->Info("created pattern with 4 quarter notes");

  for (int i = 0; i < measures; i += 1) {
    l->Info("inserting segment for measure ", i);
    if (i % 2 == 0) {
      levelAppend(g->level, fourQuarterNotes);
    } else {
      levelAppend(g->level, 4);
    }
  }
  l->Info("added beat patterns to level");

  g->levelSegment = levelGetCurrentSegment(*g->level, g->tempo);
  l->Info("got current level segment, level loading complete");

  PlayMusicStream(g->music);

  ta.TALoadTexture("bomb1");
  ta.TALoadTexture("conveyorbelt");
  ta.TALoadTexture("bg1");
  ta.TALoadTexture("bg2");
  ta.TALoadTexture("bg3");
  ta.TALoadTexture("bg4");
  ta.TALoadTexture("bg5");
  ta.TALoadTexture("bg6");
  ta.TALoadTexture("bg7");
  ta.TALoadTexture("cog1");
  ta.TALoadTexture("cog2");
  ta.TALoadTexture("cog3");
  ta.TALoadTexture("fan1");
  ta.TALoadTexture("fan2");

  GetSpriteEntity(points[0], DrawTex, AnimationState::PLAYING,
                  ta.GetTexture("bomb1"));

  GetSpriteEntity({70, 250}, DrawTex, AnimationState::DONE,
                  ta.GetTexture("cog1"), -3, 1);
  GetSpriteEntity({306, 30}, DrawTex, AnimationState::DONE,
                  ta.GetTexture("cog2"), 12, 1);
  GetSpriteEntity({645, 55}, DrawTex, AnimationState::DONE,
                  ta.GetTexture("cog3"), -5, 1);

  GetSpriteEntity({85, -5}, DrawTex, AnimationState::DONE,
                  ta.GetTexture("fan1"), -500, 2);
  GetSpriteEntity({1030, 50}, DrawTex, AnimationState::DONE,
                  ta.GetTexture("fan2"), 750, 2);

  while (!WindowShouldClose()) {
    Update();
    Draw();
    FlushEntities();
  }

  CloseWindow();
}

void Update() {
  for (Entity *e : g->entities) {
    e->Lerp(GetFrameTime());
  }

  UpdateMusicStream(g->music);

  g->level->time += GetFrameTime();

  // first switch statement determines whether or not the current segment is
  // still current
  switch (g->levelSegment.tag) {
  case LevelSegmentTag::PATTERN: {
    Pattern &p = g->levelSegment.pattern;
    p.time += GetFrameTime();
    float patternDuration = duration(p, g->tempo);
    if (p.time > patternDuration) {
      g->levelSegment = levelGetCurrentSegment(*g->level, g->tempo);
    }
    break;
  }
  case LevelSegmentTag::REST: {
    RestSegment &r = g->levelSegment.rest;
    r.time += GetFrameTime();
    float restDuration = segmentGetDuration(g->levelSegment, g->tempo);
    if (r.time > restDuration) {
      g->levelSegment = levelGetCurrentSegment(*g->level, g->tempo);
    }
    break;
  }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    g->clicks.push_back(getBeatAccuracy());
  }

  // PERF: potentially expensive to count all the beats that have passed in the
  // song and also loop through every time the player has clicked every frame
  int passedBeats = levelGetPassedBeats(*g->level, g->tempo, GOOD_THRESHOLD);
  int goodClicks = countGoodClicks();
  if (passedBeats > goodClicks) {
    // enter fail state
    l->Error("missed a spot!");
  }
}

void Draw() {
  BeginDrawing();
  ClearBackground(BLACK);

  DrawTexture(ta.GetTexture("bg1"), 0, 0, WHITE);
  DrawTexture(ta.GetTexture("bg2"), 0, 0, WHITE);

  for (Entity *e : g->entities) {
    if (!e->visible) {
      continue;
    }

    if (e->drawLayer == 1) {
      e->renderMethod(e);
    }
  }

  DrawTexture(ta.GetTexture("bg3"), 0, 0, WHITE);
  DrawTexture(ta.GetTexture("bg4"), 0, 0, WHITE);
  DrawTexture(ta.GetTexture("bg5"), 0, 0, WHITE);
  DrawTexture(ta.GetTexture("bg6"), 0, 0, WHITE);

  for (Entity *e : g->entities) {
    if (!e->visible) {
      continue;
    }

    if (e->drawLayer == 2) {
      e->renderMethod(e);
    }
  }
  DrawTexture(ta.GetTexture("bg7"), 0, 0, WHITE);

  DrawTexture(ta.GetTexture("conveyorbelt"), 0, 0, WHITE);

  for (Entity *e : g->entities) {
    if (!e->visible) {
      continue;
    }

    if (e->drawLayer == 0) {
      e->renderMethod(e);
    }
  }

  EndDrawing();
}

void FlushEntities() {
  for (Entity *e : g->entities) {
    if (e->dead) {
      DeleteEntity(e);
      continue;
    }
  }
}

void DrawRect(Entity *e) {

  DrawRectangleRec(Rectangle{e->position.x - e->size.x / 2,
                             e->position.y - e->size.y / 2, e->size.x,
                             e->size.y},
                   RED);
}

void DrawTex(Entity *e) {
  Rectangle sr = {0, 0, (float)e->texture.width, (float)e->texture.height};

  Rectangle dr = {e->position.x, e->position.y, (float)e->texture.width,
                  (float)e->texture.height};

  vec2 origin = {(float)e->texture.width / 2, (float)e->texture.height / 2};

  DrawTexturePro(e->texture, sr, dr, origin, GetTime() * e->rotSpeed, RAYWHITE);
}
