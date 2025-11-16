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

  Beat() : count(1), subdivision(QUARTER) {};
  Beat(Subdivision s) : count(1), subdivision(s) {};
  Beat(int count, Subdivision s) : count(count), subdivision(s) {};
};

struct Pattern {
  std::vector<Beat> rhythm;
  float time;

  Pattern() {
    rhythm = {};
    time = 0;
  }

  Pattern(std::initializer_list<Beat> beats) {
    rhythm = {beats};
    time = 0;
  }

  Pattern(const Pattern &other) {
    rhythm = {};
    for (auto b : other.rhythm) {
      rhythm.push_back(b);
    }
    time = other.time;
  };

  Pattern &operator=(const Pattern &other) {
    rhythm = {};
    for (auto b : other.rhythm) {
      rhythm.push_back(b);
    }
    time = other.time;
    return *this;
  };
};

struct GetBeatResult {
  int beatNumber;
  float timeSinceBeat;
};

float patternGetBeatDistance(Pattern *p, float tempo);
int patternGetPassedBeats(Pattern *p, float tempo, float threshold);

float duration(Pattern p, float tempo);
float duration(Beat b, float tempo);
float duration(Subdivision s, float tempo);

float quarters(Pattern p);
float quarters(Beat b);
float quarters(Subdivision s);

GetBeatResult getBeat(Music m, Subdivision subdivision, float tempo);

float secondsPerBeat(float tempo);

#endif
