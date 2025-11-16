#include "Level.hpp"
#include "Rhythm.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>

void levelInit(Level *lv) {
  lv->segments = NULL;
  lv->current = NULL;
  lv->len = 0;
  lv->cap = 0;
}

void levelInit(Level *lv, int cap) {
  lv->segments = (Pattern*)malloc(sizeof(Pattern) * cap);
  lv->current = lv->segments;

  lv->len = 0;
  lv->cap = cap;
}

void levelAppend(Level *lv, Pattern pattern) {
  if (lv->len + 1 > lv->cap) {
    size_t newCap = lv->cap >= 8 ? 8 : lv->cap * 2;
    Pattern *newSegments = (Pattern*)realloc(lv->segments, sizeof(Pattern) * newCap);
    lv->cap = newCap;
  }

  lv->segments[lv->len] = pattern;
  lv->len += 1;
}

bool levelIsAtEnd(Level *lv) {
  return lv->current >= lv->segments;
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
