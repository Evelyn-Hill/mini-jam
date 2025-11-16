#include "Entity.hpp"
#include "Global.hpp"
#include "Level.hpp"
#include "Rhythm.hpp"
#include "Scoring.hpp"
#include "TextureAtlas.hpp"
#include "SFXAtlas.hpp"
#include <iostream>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <unordered_map>
#include <utility>

void Update(float deltaTime);
void Draw();
void DrawRect(Entity *e);
void DrawTex(Entity *e);
void FlushEntities();
void DrawRotTex(vec2 pos, Texture2D tex, float time, float rotSpeed);
void CountQuarters();
void MimicPattern(Pattern* p);
void ListenPattern(Pattern* p);
void DrawBomb(Entity *e);
void AllocatePatterns();

static TextureAtlas ta;
//static SFXAtlas sfxa;

float pickupTime;
float pickupTimer = 0;

int main() {
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(1280, 720, "Hello, World!");
  InitAudioDevice();

  l->Info("Hello, Minijam!");
  g->tempo = 108;
  g->lastQuarter = 0;
  g->currentQuarter = 0;
  g->spawnedThisBeat = false;

  AllocatePatterns();

  g->music = LoadMusicStream("assets/save_it_redd.mp3");

  AllocatePatterns();

  g->level = new Level();
  levelInit(g->level);
  
  g->state = PlaybackState::NONE;
  int totalSongBeats =
      round(GetMusicTimeLength(g->music) / secondsPerBeat(g->tempo));
  int measures = (totalSongBeats - 1) / 4;

  for (int i = 0; i < measures; i += 1) {
    levelAppend(g->level, *g->fourQuarters);
  }

  pickupTime = secondsPerBeat(g->tempo);

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
  ta.TALoadTexture("wireSheet");

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
    Update(GetFrameTime());
    Draw();
    FlushEntities();
  }

  CloseWindow();
}

void Update(float deltaTime) {
  CountQuarters();

  if (g->spawnedThisBeat == false) {
    if (g->currentQuarter == 1) {
      Entity* e = GetSpriteEntity(points[0], DrawBomb, AnimationState::PAUSED,
                      ta.GetTexture("bomb1"));

      e->mimic = MimicPattern;
      e->listen = ListenPattern;
      e->pattern = g->fourQuarters;
      g->spawnedThisBeat = true;

      return;
    }

    if ((g->currentQuarter - 1) % 16 == 0) {

      Entity* e = GetSpriteEntity(points[0], DrawBomb, AnimationState::PAUSED,
                      ta.GetTexture("bomb1"));
      e->mimic = MimicPattern;
      e->listen = ListenPattern;
      e->pattern = g->fourQuarters;
      g->spawnedThisBeat = true;
      return;
    }
  }

  for (Entity *e : g->entities) {
    e->Lerp(deltaTime);
    e->PollBeats();
  }

  UpdateMusicStream(g->music);

  if (g->currentPattern != NULL) {
    g->currentPattern->time += deltaTime;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      g->clicks.push_back(getBeatAccuracy());
    }
  }
  
  if (g->currentPattern != NULL) {
    if (g->state == PlaybackState::MIMIC) {
      auto patternDuration = duration(*g->currentPattern, g->tempo);
      g->currentPattern->time += deltaTime; 
      if (g->currentPattern->time >= patternDuration) {
        g->state = PlaybackState::NONE;
        g->currentPattern = NULL;
        return;
      }

      l->Info("Mimic"); 
    }

    if (g->state == PlaybackState::LISTEN) {
      auto patternDuration = duration(*g->currentPattern, g->tempo);
      g->currentPattern->time += deltaTime; 
      if (g->currentPattern->time >= patternDuration) {
        g->state = PlaybackState::NONE;
        g->currentPattern = NULL;
        return;
      }

      l->Info("Listen"); 
    }
  }

  // PERF: potentially expensive to count all the beats that have passed in the
  // song and also loop through every time the player has clicked every frame
  // int passedBeats = levelGetPassedBeats(*g->level, g->tempo, GOOD_THRESHOLD);
  // int goodClicks = countGoodClicks();
  // if (passedBeats > goodClicks) {
  //   // enter fail state
  //   l->Error("missed a spot!");
  // }
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

  DrawTextureRec(ta.GetTexture("wireSheet"),
                 Rectangle{0, 0, (float)ta.GetTexture("wireSheet").width / 8,
                           (float)ta.GetTexture("wireSheet").height

                 },
                 {200, 100}, WHITE);

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

void DrawBomb(Entity *e) {
  Rectangle sr = {0, 0, (float)e->texture.width, (float)e->texture.height};

  Rectangle dr = {e->position.x, e->position.y, (float)e->texture.width,
                  (float)e->texture.height};

  vec2 origin = {(float)e->texture.width / 2, (float)e->texture.height / 2};

  DrawTexturePro(e->texture, sr, dr, origin, GetTime() * e->rotSpeed, RAYWHITE);

  Texture2D wireSheet = ta.GetTexture("wireSheet");
  int count = 0;
  for (Beat b : e->pattern->rhythm) {
      DrawTextureRec(
      wireSheet,
      Rectangle {
        0, 0,
        (float)wireSheet.width / 8, (float)wireSheet.height
      },
      { (e->position.x - 150) + (60 * count), e->position.y - 135 },
      WHITE
    );

    count++;
  }
}

void CountQuarters() {
  g->currentQuarter = getBeat(g->music, QUARTER, g->tempo).beatNumber;
  if (g->lastQuarter < g->currentQuarter) {
    g->spawnedThisBeat = false;
    g->lastQuarter = g->currentQuarter;
  }
}

void AllocatePatterns() {
  g->fourQuarters = new Pattern({
      Beat(),
      Beat(),
      Beat(),
      Beat(),
  });

  g->twoHalves = new Pattern({
      Beat(HALF),
      Beat(HALF),
  });

  g->oneAndTwoThreeAndFour = new Pattern({
      Beat(EIGHTH),
      Beat(EIGHTH),
      Beat(),
      Beat(EIGHTH),
      Beat(EIGHTH),
      Beat(),
  });

  g->oneTwoAndThreeFour = new Pattern({
      Beat(),
      Beat(EIGHTH),
      Beat(EIGHTH),
      Beat(),
      Beat(),
  });

  g->oneTwoAndThreeAndFour = new Pattern({
      Beat(),
      Beat(EIGHTH),
      Beat(EIGHTH),
      Beat(EIGHTH),
      Beat(EIGHTH),
      Beat(),
  });
}

void MimicPattern(Pattern *p) { 
  l->Info("Mimic"); 
  g->state = PlaybackState::MIMIC;
};

void ListenPattern(Pattern* p) { 
  l->Info("Pattern");
};
