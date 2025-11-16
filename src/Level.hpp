#ifndef LEVEL_H
#define LEVEL_H

#include "Rhythm.hpp"

struct Level {
  std::vector<Pattern> segments;
  Pattern *current;
};

void levelInit(Level *lv);
void levelInit(Level *lv, int cap);
void levelAppend(Level *lv, Pattern p);
bool levelIsAtEnd(Level *lv);
Pattern *levelPeek(Level *lv);
Pattern *levelAdvance(Level *lv);

#endif
