#ifndef LEVEL_H
#define LEVEL_H

#include "Rhythm.hpp"
#include <vector>

struct RestSegment {
  int beats;
  float time;
};

enum LevelSegmentTag {
  PATTERN,
  REST,
};

struct LevelSegment {
  LevelSegmentTag tag;
  union {
    Pattern pattern;
    RestSegment rest;
  };

  LevelSegment(int beats) : tag(LevelSegmentTag::REST), rest({ .beats = beats, .time = 0 }) {};

  LevelSegment(const LevelSegment &other) : tag(other.tag) {
    if (tag == LevelSegmentTag::PATTERN) {
      pattern = other.pattern;
    } else {
      rest = other.rest;
    }
  }

  LevelSegment& operator=(const LevelSegment& other) {
    if (this == &other) {
      return *this;
    }

    switch (other.tag) {
      case LevelSegmentTag::PATTERN:
        pattern = other.pattern;
        break;
      case LevelSegmentTag::REST:
        rest = other.rest;
        break;
    }
    return *this;
  };
};

struct Level {
  std::vector<LevelSegment> segments;
  float time;
};

struct GetCurrentSegmentResult {
  LevelSegment segment;
  float sinceStarted;
};

GetCurrentSegmentResult levelGetCurrentSegment(Level l, float tempo);

Level levelCreate();
void levelDestroy(Level *l);
void levelAppend(Level *l, Pattern p);
void levelAppend(Level *l, int restBeats);

float segmentGetDuration(LevelSegment s, float tempo);
int segmentGetQuarters(LevelSegment s);

#endif
