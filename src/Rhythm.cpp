#include "Rhythm.hpp"
#include "Global.hpp"
#include "raylib.h"
#include <cassert>
#include <cmath>

float patternGetBeatDistance(Pattern *p, float tempo) {
  Beat last = p->rhythm[p->rhythm.size() - 1];
  float patternDuration = duration(*p, tempo);
  float lastBeatDuration = duration(last, tempo);
  if (patternDuration - lastBeatDuration < p->time) {
    return -1;
  }

float patternGetBeatDistance(Pattern *p, float tempo) {
  Beat last = p->rhythm[p->rhythm.size() - 1];
  float patternDuration = duration(*p, tempo);
  float lastBeatDuration = duration(last, tempo);
  if (patternDuration - lastBeatDuration < p->time) {
    return -1;
  }

  float time = duration(p->rhythm[0], tempo);
  for (int i = 1; i < p->rhythm.size(); i += 1) {
    if (time > p->time) {
      Beat curr = p->rhythm[i - 1];
      float untilBeatEnd = time - p->time;
      float beatDuration = duration(curr, tempo);
      if (untilBeatEnd > beatDuration / 2) {
        return beatDuration - untilBeatEnd;
      } else {
        return untilBeatEnd;
      }
    }
    Beat curr = p->rhythm[i];
    time += duration(curr, tempo);
  }
  return -1;
}

int patternGetPassedBeats(Pattern *p, float tempo, float threshold) {
  int result = 0;
  float time = 0;
  for (auto e : p->rhythm) {
    if (time > p->time - threshold) {
      break;
    }
    result += 1;
    time += duration(e, tempo);
  }
  return result;
}

GetBeatResult getBeat(Music m, Subdivision subdivision, float tempo) {
  float timePlaying = GetMusicTimePlayed(m);
  float numBeats =
      timePlaying / (secondsPerBeat(tempo) * quarters(subdivision));
  int wholeBeats = floor(numBeats);
  float fracBeats = numBeats - wholeBeats;
  return (GetBeatResult){
      .beatNumber = wholeBeats,
      .timeSinceBeat = fracBeats * duration(subdivision, tempo),
  };
}

float duration(Pattern p, float tempo) {
  float sum = 0;
  for (Beat b : p.rhythm) {
    sum += duration(b, tempo);
  }
  return sum;
}

float duration(Beat b, float tempo) {
  return duration(b.subdivision, tempo) * b.count;
}

float duration(Subdivision s, float tempo) {
  return quarters(s) * secondsPerBeat(tempo);
}

float quarters(Pattern p) {
  float sum = 0;
  for (Beat b : p.rhythm) {
    sum += quarters(b);
  }
  return sum;
}

float quarters(Beat b) { return quarters(b.subdivision) * b.count; }

float quarters(Subdivision s) {
  float result;
  switch (s) {
  case Subdivision::WHOLE:
    result = 4.0f;
    break;
  case Subdivision::HALF:
    result = 2.0f;
    break;
  case Subdivision::QUARTER:
    result = 1.0f;
    break;
  case Subdivision::EIGHTH:
    result = 0.5f;
    break;
  }
  return result;
}

float secondsPerBeat(float tempo) { return 60.0 / tempo; }
