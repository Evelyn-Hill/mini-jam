#ifndef RHYTHM_H
#define RHYTHM_H

#include <raylib.h>
#include <vector>

enum Subdivision {
  WHOLE,
  HALF,
  QUARTER,
  EIGHTH,
};

struct Beat {
  int count;
  Subdivision subdivision;
};

struct Pattern {
  std::vector<Beat> rhythm;
  float time;

  Pattern(const Pattern &other) {
    rhythm = std::move(other.rhythm);
    time = other.time;
  };
};

Beat patternGetCurrentBeat(Pattern p, float tempo);

struct GetBeatResult {
  int beatNumber;
  float timeSinceBeat;
};

float duration(Pattern p, float tempo);
float duration(Beat b, float tempo);
float duration(Subdivision s, float tempo);

float quarters(Pattern p);
float quarters(Beat b);
float quarters(Subdivision s);

GetBeatResult getBeat(Music m, Subdivision subdivision, float tempo);

float secondsPerBeat(float tempo);

#endif
