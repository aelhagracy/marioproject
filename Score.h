// Score.h
#ifndef SCORE_H
#define SCORE_H

class Score {
public:
    Score(int maxPoints = 10);
    void addPoint();
    void reset();
    int points() const;

private:
    int point;
    int maxPoints;
};
#endif
