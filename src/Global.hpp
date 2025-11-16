#pragma once
#include "Assert.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Types.hpp"
#include "Scoring.hpp"

struct Entity;

struct Global {
  std::vector<Entity *> entities;
  Music music;
  float tempo;
  Level *level;
  Pattern *currentPattern;
  std::vector<Accuracy> clicks;
  int currentQuarter;
  int lastQuarter;
  bool spawnedThisBeat;
  Pattern *fourQuarters;
  Pattern *twoHalves;
  Pattern *oneAndTwoThreeAndFour;
  Pattern *oneTwoAndThreeFour;
  Pattern *oneTwoAndThreeAndFour;
};

extern Global *g;
extern Log *l;
