#include "Entity.hpp"
#include "Global.hpp"
#include "Rhythm.hpp"
#include "TextureAtlas.hpp"
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

static TextureAtlas ta;

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

  pickupTime = secondsPerBeat(g->tempo);

  g->music = LoadMusicStream("assets/save_it_redd.mp3");
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
      Entity* e = GetSpriteEntity(points[0], DrawTex, AnimationState::PAUSED,
                      ta.GetTexture("bomb1"));

      e->mimic = MimicPattern;
      e->listen = ListenPattern;
      g->spawnedThisBeat = true;

      return;
    }

    if ((g->currentQuarter - 1) % 16 == 0) {

      Entity* e = GetSpriteEntity(points[0], DrawTex, AnimationState::PAUSED,
                      ta.GetTexture("bomb1"));
      e->mimic = MimicPattern;
      e->listen = ListenPattern;

      g->spawnedThisBeat = true;
      return;
    }
  }

  for (Entity *e : g->entities) {
    e->Lerp(deltaTime);
    e->PollBeats();
  }

  UpdateMusicStream(g->music);
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

  DrawTextureRec(ta.GetTexture("wireSheet"), Rectangle {
    0, 0,
    (float)ta.GetTexture("wireSheet").width / 8, (float)ta.GetTexture("wireSheet").height

  }, {200, 100}, WHITE);

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
      { e->position.x + (20 * count), e->position.y + (20 * count) },
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


void MimicPattern(Pattern* p) {
  l->Info("Mimic");
};

void ListenPattern(Pattern* p){
  l->Info("Pattern");
};
