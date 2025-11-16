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
    RestSegment rest;
    Pattern pattern;
  };

  LevelSegment() : tag(REST), rest({.beats = 0, .time = 0}) {};

  LevelSegment(Pattern p) : tag(PATTERN) {
    pattern.rhythm = p.rhythm;
    pattern.time = p.time;
  }

  LevelSegment(int beats) : tag(REST) {
    rest = {
        .beats = beats,
        .time = 0,
    };
  }

  LevelSegment(const LevelSegment &other) : tag(other.tag) {
    switch (tag) {
    case PATTERN:
      pattern = other.pattern;
      break;
    case REST:
      rest = other.rest;
      break;
    }
  }

  LevelSegment &operator=(const LevelSegment &other) {
    tag = other.tag;
    switch (tag) {
    case PATTERN:
      pattern = other.pattern;
      break;
    case REST:
      rest = other.rest;
      break;
    }
    return *this;
  }

  ~LevelSegment() {};
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
