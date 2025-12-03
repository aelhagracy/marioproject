
#include "Score.h"

Score::Score(int maxPoints) : point(0), maxPoints(maxPoints) {}

void Score::addPoint() {
    if (point < maxPoints) ++point;
}

void Score::reset() {
    point = 0;
}

int Score::points() const {
    return point;
}
