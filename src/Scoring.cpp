#include "Scoring.hpp"
#include "Global.hpp"

Accuracy getBeatAccuracy() {
  float sinceBeat, beatDuration;
  switch (g->levelSegment.tag) {
  case LevelSegmentTag::PATTERN: {
    Beat currentBeat = patternGetCurrentBeat(g->levelSegment.pattern, g->tempo);
    GetBeatResult beatData =
        getBeat(g->music, currentBeat.subdivision, g->tempo);
    sinceBeat = beatData.timeSinceBeat;
    beatDuration = duration(currentBeat, g->tempo);
    break;
  }
  case LevelSegmentTag::REST:
    return Accuracy::MISS;
  }

  float beatDistance;
  if (sinceBeat < beatDuration / 2) {
    beatDistance = sinceBeat;
  } else {
    beatDistance = beatDuration - sinceBeat;
  }

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
