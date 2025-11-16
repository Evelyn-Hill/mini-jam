#pragma once
#include "Entity.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include <vector>
<<<<<<< HEAD
=======

struct Entity;
>>>>>>> 2c582bb (Added texutre atlas)

struct Global {
  std::vector<Entity *> entities;
  Music music;
  float tempo;
  Level level;
  LevelSegment levelSegment;
};

extern Global *g;
extern Log *l;
