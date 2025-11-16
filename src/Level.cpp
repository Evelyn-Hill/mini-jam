#include "Level.hpp"
#include "Rhythm.hpp"
#include <vector>

GetCurrentSegmentResult levelGetCurrentSegment(Level l, float tempo) {
  float time = segmentGetDuration(l.segments[0], tempo);
  for (int i = 1; i < l.segments.size(); i += 1) {
    if (time > l.time) {
      LevelSegment segment = l.segments[i - 1];
      float duration = segmentGetDuration(segment, tempo);
      float sinceStarted = duration - (time - l.time);
      return {.segment = segment, .sinceStarted = sinceStarted};
    }
    LevelSegment segment = l.segments[i];
    time += segmentGetDuration(segment, tempo);
  }
  return {
      .segment = l.segments[l.segments.size() - 1],
      .sinceStarted = l.time - time,
  };
}

Level levelCreate() {
  std::vector<LevelSegment> segments;
  return (Level){
      .segments = segments,
      .time = 0,
  };
}

void levelAppend(Level *l, Pattern pattern) {
  l->segments.push_back(LevelSegment(pattern));
}

void levelAppend(Level *l, int beats) {
  l->segments.push_back(LevelSegment(beats));
}

int levelGetPassedBeats(Level l, float tempo) {
  int result = 0;
  float time = 0;
  for (auto s : l.segments) {
    if (time > l.time) {
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
