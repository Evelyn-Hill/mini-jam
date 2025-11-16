#include "Scoring.hpp"
#include "Global.hpp"
#include "Rhythm.hpp"

Accuracy getBeatAccuracy() {
  float beatDistance = patternGetBeatDistance(g->currentPattern, g->tempo);

  if (beatDistance < GOOD_THRESHOLD) {
    return Accuracy::GOOD;
  } else {
    return Accuracy::MISS;
  }
}

int countGoodClicks() {
  int result = 0;
  for (auto a : g->clicks) {
    if (a != MISS) {
      result += 1;
    }
  }
  return result;
}
