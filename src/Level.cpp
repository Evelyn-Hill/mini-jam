#include "Level.hpp"
#include "Rhythm.hpp"
#include "Global.hpp"
#include <vector>

LevelSegment::LevelSegment() : tag(REST), rest({.beats = 0, .time = 0}) {};

LevelSegment::LevelSegment(Pattern p) : tag(PATTERN) {
    pattern.time = p.time;
    pattern.rhythm = p.rhythm;
    l->Info("copied pattern rhythm");
  }

LevelSegment::LevelSegment(int beats) : tag(REST) {
    rest = {
        .beats = beats,
        .time = 0,
    };
  }

LevelSegment::LevelSegment(const LevelSegment &other) : tag(other.tag) {
    switch (tag) {
    case PATTERN:
      pattern = other.pattern;
      break;
    case REST:
      rest = other.rest;
      break;
    }
  }


LevelSegment::~LevelSegment() {};

LevelSegment levelGetCurrentSegment(Level l, float tempo) {
  float time = segmentGetDuration(l.segments[0], tempo);
  for (int i = 1; i < l.segments.size(); i += 1) {
    if (time > l.time) {
      LevelSegment segment = l.segments[i - 1];
      float duration = segmentGetDuration(segment, tempo);
      float sinceStarted = duration - (time - l.time);
      switch (segment.tag) {
      case LevelSegmentTag::PATTERN:
        segment.pattern.time = sinceStarted;
      case LevelSegmentTag::REST:
        segment.rest.time = sinceStarted;
      }
      return segment;
    }
    LevelSegment segment = l.segments[i];
    time += segmentGetDuration(segment, tempo);
  }
  LevelSegment segment = l.segments[l.segments.size() - 1];
  switch (segment.tag) {
  case LevelSegmentTag::PATTERN:
    segment.pattern.time = l.time - time;
  case LevelSegmentTag::REST:
    segment.rest.time = l.time - time;
  }
  return segment;
}

void levelAppend(Level *lv, Pattern pattern) {
  l->Info("appending pattern to level");
  lv->segments.push_back(LevelSegment(pattern));
}

void levelAppend(Level *lv, int beats) {
  l->Info("appending ", beats, "-beat rest to level");
  lv->segments.push_back(LevelSegment(beats));
}

int levelGetPassedBeats(Level l, float tempo, float threshold) {
  int result = 0;
  float time = 0;
  for (auto s : l.segments) {
    if (time - threshold > l.time) {
      break;
    }
    if (s.tag == LevelSegmentTag::REST) {
      time += segmentGetDuration(s, tempo);
      continue;
    }
    for (auto b : s.pattern.rhythm) {
      if (time > l.time) {
        break;
      }

      result += 1;
      time += duration(b, tempo);
    }
  }
  return result;
}

float segmentGetDuration(LevelSegment s, float tempo) {
  float result;
  switch (s.tag) {
  case LevelSegmentTag::PATTERN:
    result = duration(s.pattern, tempo);
    break;
  case LevelSegmentTag::REST:
    result =
        duration((Beat){.count = s.rest.beats, .subdivision = QUARTER}, tempo);
    break;
  }
  return result;
}

int segmentGetQuarters(LevelSegment s) {
  int result;
  switch (s.tag) {
  case LevelSegmentTag::PATTERN:
    result = quarters(s.pattern);
    break;
  case LevelSegmentTag::REST:
    result = quarters((Beat){.count = s.rest.beats, .subdivision = QUARTER});
    break;
  }
  return result;
}
