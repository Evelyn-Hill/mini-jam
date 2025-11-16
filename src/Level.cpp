#include "Level.hpp"
#include "Rhythm.hpp"

void levelInit(Level *lv) {
  lv->current = &lv->current[0];
}

void levelAppend(Level *lv, Pattern pattern) {
  lv->segments.push_back(pattern);
}

bool levelIsAtEnd(Level *lv) {
  return lv->current >= &lv->segments[lv->segments.size() - 1];
}

Pattern *levelPeek(Level *lv) {
  if (levelIsAtEnd(lv)) {
    return nullptr;
  }
  return lv->current;
}

Pattern *levelAdvance(Level *lv) {
  if (levelIsAtEnd(lv)) {
    return nullptr;
  }
  Pattern *result = lv->current;
  lv->current += 1;
  return result;
}
