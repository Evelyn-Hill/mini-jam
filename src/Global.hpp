#pragma once
#include "Assert.hpp"
#include "Entity.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Types.hpp"

struct Global {
  std::vector<Entity *> entities;
  Music music;
  float tempo;
  Level level;
  LevelSegment levelSegment;
};

extern Global *g;
extern Log *l;
