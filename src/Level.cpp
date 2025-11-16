#include "Level.hpp"
#include <vector>

GetCurrentSegmentResult levelGetCurrentSegment(Level l, float tempo) {
  l.segments[0] = LevelSegment(3);
  float time = segmentGetDuration(l.segments[0], tempo);
  for (int i = 1; i < l.segments.size(); i += 1) {
    if (time > l.time) {
      LevelSegment segment = l.segments[i - 1];
      float duration = segmentGetDuration(segment, tempo);
      float sinceStarted = duration - (time - l.time);
      return (GetCurrentSegmentResult){
          .segment = segment,
          .sinceStarted = sinceStarted,
      };
    }
    LevelSegment segment = l.segments[i];
    time += segmentGetDuration(segment, tempo);
  }
  return (GetCurrentSegmentResult){
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
  LevelSegment segment = (LevelSegment){
    .tag = LevelSegmentTag::PATTERN,
    .pattern = pattern,
  };
  l->segments.push_back(segment); 
}

void levelAppend(Level *l, int beats) {
  l->segments.push_back((RestSegment){.beats = beats, .time = 0});
}

float segmentGetDuration(LevelSegment s, float tempo) {
}
