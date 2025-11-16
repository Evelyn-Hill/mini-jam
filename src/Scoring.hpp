#ifndef SCORING_H
#define SCORING_H

#define GOOD_THRESHOLD 0.0825

enum Accuracy {
    NONE,
    MISS,
    GOOD,
};

Accuracy getBeatAccuracy();

#endif
