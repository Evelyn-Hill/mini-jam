#include "Assert.hpp"
#include "Level.hpp"
#include "Log.hpp"
#include "Types.hpp"

struct Global {
  const char *name = "Hi";
  Music music;
  float tempo;
  Level level;
  LevelSegment levelSegment;
};

extern Global *g;
extern Log *l;
