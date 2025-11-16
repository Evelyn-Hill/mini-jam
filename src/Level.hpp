#ifndef LEVEL_H
#define LEVEL_H

#include "Rhythm.hpp"
#include <cstddef>

struct Level {
  Pattern *segments;
  Pattern *current;
  size_t len;
  size_t cap;
};

void levelInit(Level *lv);
void levelInit(Level *lv, int cap);
void levelAppend(Level *lv, Pattern p);
bool levelIsAtEnd(Level *lv);
Pattern *levelPeek(Level *lv);
Pattern *levelAdvance(Level *lv);

#endif
